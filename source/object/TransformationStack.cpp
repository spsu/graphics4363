#include "TransformationStack.hpp"
#include "../libs/math.hpp"
#include <stdio.h>

TransformationStack::TransformationStack():
	matrixStack(),
	vRot(),
	vScale(),
	vTrans()
{
	GLfloat* first = math::newIdentityMat(); 
	matrixStack.push(first);
}

void TransformationStack::push()
{
	matrixStack.push(copyTop());

	// Pending state changes no longer apply
	vRot = Vertex();
	vScale = Vertex();
	vTrans = Vertex();
}

void TransformationStack::pop()
{
	GLfloat* prev(0);

	prev = matrixStack.top();
	matrixStack.pop();

	// Pending state changes no longer apply
	vRot = Vertex();
	vScale = Vertex();
	vTrans = Vertex();

	delete prev; // XXX: Careful. Warn about shared ptr in doc.
}

unsigned int TransformationStack::size()
{
	return matrixStack.size();
}

GLfloat* TransformationStack::top()
{
	return matrixStack.top();
}

GLfloat* TransformationStack::copyTop()
{
	// XXX: Must deallocate. 
	return math::copyMat(matrixStack.top());	
}

void TransformationStack::rotate(GLfloat x, GLfloat y, GLfloat z)
{
	vRot = Vertex(x, y, z);
}

void TransformationStack::scale(GLfloat x, GLfloat y, GLfloat z)
{
	vScale = Vertex(x, y, z);
}

void TransformationStack::translate(GLfloat x, GLfloat y, GLfloat z)
{
	vTrans = Vertex(x, y, z);
}


void TransformationStack::applyTransform()
{
	GLfloat* top = matrixStack.top();
	GLfloat* newTop = new GLfloat[16];

	// Intermediate matrices. 
	GLfloat* rotX = new GLfloat[16];
	GLfloat* rotY = new GLfloat[16];
	GLfloat* rotZ = new GLfloat[16];
	GLfloat* rotXY = new GLfloat[16];
	GLfloat* rotXYZ = new GLfloat[16];
	GLfloat* trans = new GLfloat[16];
	GLfloat* rotTrans = new GLfloat[16];

	// Rotation
	math::rotateX(rotX, vRot.x);
	math::rotateY(rotY, vRot.y);
	math::rotateZ(rotZ, vRot.z);

	printf("XZY: %f, %f, %f\n", vRot.x, vRot.y, vRot.z);

	math::matrixMult4x4(rotXY, rotX, rotY);
	math::matrixMult4x4(rotXYZ, rotXY, rotZ);

	// Translation. 
	math::translate(trans, vTrans.x, vTrans.y, vTrans.z);

	// Combine. 
	//math::matrixMult4x4(rotTrans, trans, rotXYZ);
	math::matrixMult4x4(rotTrans, rotXYZ, trans);

	// Final combine. 
	//math::matrixMult4x4(newTop, top, rotTrans);
	math::matrixMult4x4(newTop, rotTrans, top);

	matrixStack.top() = newTop;

	// State changes were applied, so reset the pending state buffers
	vRot = Vertex();
	vScale = Vertex();
	vTrans = Vertex();

	// TODO: Cleanup ALL state.
	// TODO: Only create matrices and do matrix math when absolutely 
	// necessary to reduce overhead. 

	delete top;
}


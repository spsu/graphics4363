#ifndef BT_GRAPHICS_OBJECT_TRANSFORMATIONSTACKREGISTRY
#define BT_GRAPHICS_OBJECT_TRANSFORMATIONSTACKREGISTRY

#include <GL/glew.h>
#include <GL/glut.h>

class TransformationStack;

/**
 * TODO: Doc
 */
class TransformationStackRegistry
{
	public:
		// Get instance
		static TransformationStack* get();

	private:
		// XXX: This is a singleton!
		TransformationStackRegistry() {};
		TransformationStackRegistry(const TransformationStackRegistry&) {};
		TransformationStackRegistry& operator=(
				const TransformationStackRegistry&); 

		// Instance
		static TransformationStack* stack;
};

#endif

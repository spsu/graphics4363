#include "TransformationStackRegistry.hpp"
#include "TransformationStack.hpp"

TransformationStack* TransformationStackRegistry::stack = 0;

TransformationStack* TransformationStackRegistry::get()
{
	if(stack == NULL) {
		stack = new TransformationStack();	
	}
	return stack;
}


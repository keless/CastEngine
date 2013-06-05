#ifndef _CASTOBJECT_H_
#define _CASTOBJECT_H_

#include <string>

class CastTarget;

class ICastEntity 
{
public:

	virtual void setPropertyF( std::string propName, float value ) {}
	virtual float* getPropertyF( std::string propName ) { return NULL; }
	virtual void setPropertyI( std::string propName, int value ) {}
	virtual int* getPropertyI( std::string propName )  { return NULL; }

	CastTarget* getTarget() { return NULL; }
};

#endif

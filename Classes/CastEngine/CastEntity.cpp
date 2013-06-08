#include "CastEntity.h"

#include "CastEntityPointer.h"

void ICastEntity::detachAllPointers()
{
	m_CastPointers.clear();
}
void ICastEntity::attachPointer( CastEntityPointer* ptr )
{
	m_CastPointers.push_back(ptr);
}
void ICastEntity::detachPointer( CastEntityPointer* ptr )
{
	m_CastPointers.erase( std::find(m_CastPointers.begin(), m_CastPointers.end(), ptr) );
}
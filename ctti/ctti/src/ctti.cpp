#include "../ctti.h"

unsigned TypeDesc::getDepth() const
{
    // stub
    return 0;
}

TypeDesc const * const * TypeDesc::getParents() const
{
    // stub
    return NULL;
}

bool TypeDesc::operator==(TypeDesc const &td) const
{
    return this == &td;
}

bool TypeDesc::operator!=(TypeDesc const &td) const
{
    return !(*this == td);
}

bool TypeDesc::isSubclassOf(TypeDesc const &td) const
{
    return  this->getDepth() >= td.getDepth() &&
            *(this->getParents()[ td.getDepth() ]) == td;
}

Object::TypeDescObject::TypeDescObject()
{
    parents[depth] = this;
}

unsigned Object::TypeDescObject::getDepth() const
{
    return depth;
}

TypeDesc const * const * Object::TypeDescObject::getParents() const
{
    return parents;
}

TypeDesc const & Object::getType()
{
    static TypeDescObject td;
    return td;
}


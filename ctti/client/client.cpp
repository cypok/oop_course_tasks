#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>

#include "../ctti/ctti.h"

class A : public Object
{
    CTTI_OBJECT(A, Object)
};

class B : public A
{
    CTTI_OBJECT(B, A)
};

class X : public Object
{
    CTTI_OBJECT(X, Object)
};

int main()
{
    A a;
    B b;
    X x;

    TypeDesc const &atd = a.getType();
    TypeDesc const &btd = b.getType();
    TypeDesc const &xtd = x.getType();

    assert(   btd.isSubclassOf( atd ) );
    assert( ! xtd.isSubclassOf( atd ) );
    return 0;
}

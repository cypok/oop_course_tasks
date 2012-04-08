#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#define CTTI_OBJECT(name, parent) \
public: \
    class TypeDesc##name : public TypeDesc \
    { \
    public: \
        enum { depth = parent::TypeDesc##parent::depth + 1 }; \
    private: \
        TypeDesc const *parents[depth + 1]; \
    public: \
        TypeDesc##name() \
        { \
            memcpy(parents, parent::getType().getParents(), depth*sizeof(parents[0])); \
            parents[depth] = this; \
        } \
        virtual unsigned getDepth() const { return depth; } \
        virtual TypeDesc const * const * getParents() const { return parents; } \
    }; \
    static TypeDesc const & getType() \
    { \
        static TypeDesc##name td; \
        return td; \
    }

class TypeDesc
{
public:
    virtual unsigned getDepth() const;
    virtual TypeDesc const * const * getParents() const;

    inline bool operator==(TypeDesc const &td) const;
    inline bool operator!=(TypeDesc const &td) const;

    bool isSubclassOf(TypeDesc const &td) const;
};

class Object
{
public:
    class TypeDescObject : public TypeDesc
    {
    public:
        enum { depth = 0 };

    private:
        TypeDesc const *parents[depth + 1];

    public:
        TypeDescObject();

        virtual unsigned getDepth() const;
        virtual TypeDesc const * const * getParents() const;
    };

    static TypeDesc const & getType();
};


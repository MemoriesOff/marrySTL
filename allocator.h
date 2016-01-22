#ifndef ALLOCATOR_H_INCLUDED
#define ALLOCATOR_H_INCLUDED

namespace marrySTL
{
    /*
    一级空间配置器
    */
    template <typename T>
    class allocator
    {
    public:
        typedef T   value_type;
        typedef T*  pointer;
        typedef T&  reference;
        typedef const T* const_pointer;
        typedef const T& const_reference;
        typedef size_t size_type;
        template <typename U>struct rebine
        {
            typedef allocator<U> other;
        };
        allocator(){}
        ~allocator(){}
        pointer address(reference x) const noexcept
        {
            return &x;
        }
        const_pointer address(const_reference x) const noexcept
        {
            return &x;
        }
        pointer allocate(size_type num, const_pointer hint=0)
        {
            return (pointer)::operator new(num*sizeof(T));
        }
        void deallocate(pointer p,size_type n)
        {
            ::operator delete(p);
        }
        void construct(pointer p, const_reference val)
        {
            new(p) T(val);
        }
        void destroy(pointer p)
        {
            p->~T();
        }
        size_type max_size() const throw()
        {
            return static_cast<size_type>(-1) /sizeof(T);
        }
        bool operator==(allocator const& a) const
        {
            return true;
        }
        bool operator!=(allocator const& a) const
        {
            return false;
        }
    };
}

#endif // ALLOCATOR_H_INCLUDED

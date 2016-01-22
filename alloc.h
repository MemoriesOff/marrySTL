#ifndef ALLOC_H_INCLUDED
#define ALLOC_H_INCLUDED
#include <cstdlib>
#include "allocator.h"
#include "stl_construct.h"
namespace marrySTL
{
    class simple_alloc
    {
    public:
        enum{__ALIGN=8};//区块边界
        enum{__MAX_BATY=128};//区块上限
        enum{__NFREELISTS=__MAX_BATY/__ALIGN};//free-lists数目
    private:
        //结点构造
        union obj
        {
            union obj * next;
            char client[1];
        };
        //节点定义
        static obj* free_list [__NFREELISTS];
        static char *start_free;//内存池起始位置
        static char *end_free;//内存池结束位置
        static size_t heap_size;
        //上调至8的倍数
        //位操作,加7后后3位置0
        static size_t ROUND_UP(size_t bytes)
        {
            return (bytes+__ALIGN-1)&~(__ALIGN-1);
        }
        //获取free-list数组下标
        //加7保证内存不溢出
        static size_t FREELIST_INDEX(size_t bytes)
        {
            return ((bytes+__ALIGN-1)/__ALIGN-1);
        }

        static void *refill(size_t n)
        {
            obj* result;
            obj* now_obj;
            obj* next_obj;
            int nobjs=20;
            result=(obj*)chunk_alloc(n,nobjs);
            if(nobjs==1)
                return(result);
            obj* myFreeList=(obj*)(free_list+ROUND_UP(n));
             myFreeList->next=(obj *)((void*)result+n);
            next_obj=myFreeList->next;
            for(;nobjs>1;nobjs--)
            {
                now_obj=next_obj;
                next_obj=(obj *)((void*)next_obj+n);
                now_obj->next=next_obj;
            }
            now_obj->next=0;
            return(result);
        }
        static char *chunk_alloc(size_t size,int &nobjs)
        {
            size_t need=nobjs*size;
            size_t left=end_free-start_free;
            char * result;
            if(left>=need)
            {
                result=start_free;
                start_free+=need;
                return(result);
            }else if(left>=size)
            {
                nobjs=left/size;
                result=start_free;
                need=size*nobjs;
                start_free+=need;
                return(result);
            }else
            {
                size_t bytes_to_get=2*need;
                if(left>0)
                {
                    obj* myFreeList=(obj*)(free_list+ROUND_UP(left));
                    obj* q=(obj*)start_free;
                    q->next=myFreeList->next;
                    myFreeList->next=(obj*)start_free;
                }
                start_free=(char *)malloc(bytes_to_get);
                end_free=start_free+bytes_to_get;
                return(chunk_alloc(size,nobjs));
            }
        }

    public:
        static void * allocate(size_t n)
        {
            if(n>__MAX_BATY)
                return(malloc(n));
            obj*myFreeList=(obj*)(free_list+ROUND_UP(n));
            obj*result=myFreeList->next;
            if(result==0)
            {
                //重新填充
                void *r=refill(ROUND_UP(n));
                return(r);
            }
            myFreeList->next=result->next;
            return (result);
        }
        static void deallocate(void *p,size_t n)
        {
            if(n>__MAX_BATY)
            {
                free(p);
            }
            obj* myFreeList=(obj*)(free_list+ROUND_UP(n));
            obj* q;
            q=(obj*)p;
            q->next=myFreeList->next;
            myFreeList->next=q;

        }
        static void * reallocate(void *p,size_t old_sz,size_t new_sz);

    };

        simple_alloc::obj* simple_alloc::free_list [__NFREELISTS];
        char * simple_alloc::start_free;//内存池起始位置
        char * simple_alloc::end_free;//内存池结束位置
        size_t simple_alloc::heap_size;



    //对二级空间配置器的封装
    template <class T>
    class alloc
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
            typedef alloc<U> other;
        };
        alloc(){}
        ~alloc(){}
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
            return ((pointer)simple_alloc::allocate((num*sizeof(T))));
        }
        void deallocate(pointer p,size_type n)
        {
             simple_alloc::deallocate(p,n*sizeof(T));
        }
        void construct(pointer p, const_reference val)
        {
            marrySTL::construct(p,val);
        }
        void destroy(pointer p)
        {
            marrySTL::destroy(p);
        }
        size_type max_size() const throw()
        {
            return static_cast<size_type>(-1) /sizeof(T);
        }
        bool operator==(alloc const& a) const
        {
            return true;
        }
        bool operator!=(alloc const& a) const
        {
            return false;
        }
    };



}

#endif // ALLOC_H_INCLUDED

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include "alloc.h"
#include "stl_construct.h"

namespace marrySTL
{
    template<class T,class Alloc=alloc<T>>
    class vector
    {
    public:
        //形别定义
        typedef T   value_type;
        typedef T*  pointer;
        typedef T*  iterator;
        typedef T&  reference;
        typedef const T* const_pointer;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t diffrence_type;
    protected:

        Alloc date_allocator;  //空间配置器
        pointer start;   //使用空间头
        pointer finish;  //使用空间尾
        pointer end_of_storage; //可用空间尾
    public:
        //析构函数
        ~vector()
        {
            destroy(start,finish);
            date_allocator.deallocate(start,end_of_storage-start);
        }
        vector():start(0),finish(0),end_of_storage(0){};

        vector(size_type n)
        {
            start= date_allocator.allocate(n);
            pointer first=start;
            finish=first+n;
            end_of_storage=first+n;
        }

        vector(size_type n,const T& value)
        {
            start= date_allocator.allocate(n);
            pointer first=start;
            for(;n>0;n--)
            {
                *first= value;
                first++;
            }
            end_of_storage=first;
            finish=end_of_storage;
        }

        iterator begin()
        {
            return start;
        }

        const_pointer begin()const
        {
            return start;
        }


        iterator end()
        {
            return finish;
        }

        const_pointer end()const
        {
            return finish;
        }


        size_type size() const
        {
            return finish-start;
        }



        void insert(iterator position,int n,const_reference x)
        {
            pointer nowstart;
            pointer nowfinish;
            pointer i,j;
            int k;
            if((finish+n)>end_of_storage)
            {
                nowstart=date_allocator.allocate(size()+n);
                nowfinish=nowstart;
                i=start;
                for(;i<position;i++,nowfinish++)
                {
                    *nowfinish=*i;
                }
                for(k=0;k<n;k++)
                {
                    *nowfinish=x;
                    nowfinish++;
                }
                for(;i!=finish;i++)
                {
                    *nowfinish=*i;
                    nowfinish++;
                }
                destroy(start,finish);
                date_allocator.deallocate(start,end_of_storage-start);
                start=nowstart;
                finish=nowfinish;
                end_of_storage=finish;
            }else{
                nowstart=position;
                nowfinish=++finish;
                for(;nowfinish!=nowstart;nowfinish--)
                {

                    *nowfinish=*(nowfinish-1);
                }
                *nowfinish=x;
            }
        }
        void pop_back()
        {
            finish--;
        }



        reference operator[](size_type n)
        {
            return *(start+n);
        }

        const_reference operator[] (size_type n) const
        {
            return  *(start+n);
        }

        void push_back(const T& val)
        {
            insert(finish,1,val);
        }

        iterator erase ( iterator position )
        {
            pointer nowfinish,nowstart;
            nowstart=position;
            nowfinish=--finish;
            for(;nowfinish!=nowstart;nowstart++)
            {
                *nowstart=*(nowstart+1);
            }
            return position;
        }
        iterator erase ( iterator first, iterator last )
        {
            pointer nowfinish,nowstart;
            int num=last-first+1;
            nowstart=first;
            finish=finish-num;
            nowfinish=finish;
            for(;nowfinish!=nowstart;nowstart++)
            {
                *nowstart=*(nowstart+num);
            }
            return first;
        }

    };
}


#endif // VECTOR_H_INCLUDED

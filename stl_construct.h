#ifndef STL_CONSTRUCT_H_INCLUDED
#define STL_CONSTRUCT_H_INCLUDED
#include <new>
namespace marrySTL
{
    /*
    定义全局函数construct()和destroy()负责对象的构造和析构
    */
    //函数名:construct()
    //功能:接受一个已分配好的内存，在其上以value为构造函数参数的来构造一个T1对象
    //传入参数:内存指针,构造初值
    //传出参数:无
    template<class T1,class T2>
    inline void construct(T1* p,const T2& value)
    {
        new(p) T1(value);
    }
    //函数名:destroy()
    //功能:单个对象析构
    //传入参数:内存指针
    //传出参数:无
    template<class T>
    inline void destroy(T* p)
    {
        p->~T();
    }


    //函数名:destroy()
    //功能:区间内所有对象析构
    //传入参数:内存起始指针,内存终止指针
    //传出参数:无
    template<class ForwardIterator>
    inline void destroy(ForwardIterator first,ForwardIterator last)
    {
       //__destroy(fisrt,last,value_type(first));
       //暂时不作特化
       for(;first<last;first++)
       {
           destroy(&*first);//因first是迭代器，*first取出其真正内容，然后再用&取地址
       }

    }
    /*
    template<class ForwardIterator,class T>
    inline void __destory(ForwardIterator first, ForwardIterator last, T*)
    {
       typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
       __destory_aux(first, last, trivial_destructor());
    }

    //需要调用析构函数
    template<class ForwardIterator>
    inline void __destroy_aux(ForwardIterator fisrt,ForwardIterator last,__false_type)
    {
        for(;first<last;first++)
       {
           destroy(&*first)//因first是迭代器，*first取出其真正内容，然后再用&取地址
       }
    }
    //无需调用析构函数
    template<class ForwardIterator>
    inline void __destroy_aux(ForwardIterator fisrt,ForwardIterator last,__true_type)
    {
    }
    */

    template<class T>
    inline void destroy(char* first,char* last)
    {
    }


    template<class T>
    inline void destroy(wchar_t* first,wchar_t* last)
    {
    }

}

#endif // STL_CONSTRUCT_H_INCLUDED

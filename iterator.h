#ifndef ITERATOR_H_INCLUDED
#define ITERATOR_H_INCLUDED

#include <cstddef>
namespace marrySTL
{


    //五种迭代器类型
    struct input_iterator_tag{};//只读
    struct output_iterator_tag{};//只写;
    struct forward_iterator_tag:public input_iterator_tag{};//写入型
    struct bidirectional_iterator_tag:public forward_iterator_tag{};//双向操作
    struct random_access_iterator_tag:public bidirectional_iterator_tag{};//涵盖原生态指针能力

    //迭代器模板
    template <class Category,class T,class Distance=ptr,class Point=T*,class Reference=T&>
    struct iterator
    {
        typedef T value_type;
        typedef Distance difference_type;
        typedef Point pointer;
        typedef Reference reference;
        typedef Category iterator_category;
    };

    template<class Iterator>
    struct iterator_traits
    {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::reference reference;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::value_type;
    };
    //指针特化
    template<class T>
    struct iterator_traits<T*>
    {
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
        typedef random_access_iterator_tag iterator_category;
    };
    //常量指针特化
    template<class T>
    struct iterator_traits<const T*>
    {
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef const T* pointer;
        typedef const T& reference;
        typedef random_access_iterator_tag iterator_category;
    };
    //获取迭代器类型
    template<class Iterator>
    typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&)
    {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }
    // 获取迭代器差值类型 distance type
    template <class Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&)
    {
      return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    // 获取迭代器值类型 value_type
    template <class Iterator>
    inline typename iterator_traits<Iterator>::value_type*
    distance_type(const Iterator&)
    {
      return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }
    //advance实现

    //原生态指针特化
    template <class InputIterator,class Distance>
    void __advance(InputIterator& it,Distance n, random_access_iterator_tag)
    {
        it=it+n;
    }
    //双向操作指针特化
    template <class InputIterator,class Distance>
    void __advance(InputIterator& it,Distance n, bidirectional_iterator_tag)
    {
        if(n>0)
        {
            while(n!=0)
            {
                it++;
                n--;
            }
        }else
        {
            while(n!=0)
            {
                it--;
                n++;
            }
        }
    }


    //单向操作指针特化
    template <class InputIterator,class Distance>
    void __advance(InputIterator& it,Distance n, input_iterator_tag)
    {
            while(n!=0)
            {
                it++;
                n--;
            }
    }


    template <class InputIterator,class Distance>
    void advance(InputIterator& it,Distance n)
    {
        __advance(InputIterator& it,Distance n,iterator_category(it));
    }

    //distance实现

    //对原生态指针特化
    template<class InputIterator,class Distance>
    void __distance(InputIterator first,InputIterator last,Distance &n,random_access_iterator_tag)
    {
        n+=last-first;
    }

    //泛化版本
    template<class InputIterator,class Distance>
    void __distance(InputIterator first,InputIterator last,Distance &n,input_iterator_tag)
    {
        while(first!=last)
        {
            first++;
            n++;
        }
    }

    template<class InputIterator,class Distance>
    void distance(InputIterator first,InputIterator last,Distance &n)
    {
        __distance(first, last,n,iterator_category(first));
    }




}
#endif // ITERATOR_H_INCLUDED

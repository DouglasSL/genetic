               :// List implementation -*- C++ -*-
               :
               :// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
               :// Free Software Foundation, Inc.
               ://
               :// This file is part of the GNU ISO C++ Library.  This library is free
               :// software; you can redistribute it and/or modify it under the
               :// terms of the GNU General Public License as published by the
               :// Free Software Foundation; either version 2, or (at your option)
               :// any later version.
               :
               :// This library is distributed in the hope that it will be useful,
               :// but WITHOUT ANY WARRANTY; without even the implied warranty of
               :// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
               :// GNU General Public License for more details.
               :
               :// You should have received a copy of the GNU General Public License along
               :// with this library; see the file COPYING.  If not, write to the Free
               :// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
               :// USA.
               :
               :// As a special exception, you may use this file as part of a free software
               :// library without restriction.  Specifically, if other files instantiate
               :// templates or use macros or inline functions from this file, or you compile
               :// this file and link it with other files to produce an executable, this
               :// file does not by itself cause the resulting executable to be covered by
               :// the GNU General Public License.  This exception does not however
               :// invalidate any other reasons why the executable file might be covered by
               :// the GNU General Public License.
               :
               :/*
               : *
               : * Copyright (c) 1994
               : * Hewlett-Packard Company
               : *
               : * Permission to use, copy, modify, distribute and sell this software
               : * and its documentation for any purpose is hereby granted without fee,
               : * provided that the above copyright notice appear in all copies and
               : * that both that copyright notice and this permission notice appear
               : * in supporting documentation.  Hewlett-Packard Company makes no
               : * representations about the suitability of this software for any
               : * purpose.  It is provided "as is" without express or implied warranty.
               : *
               : *
               : * Copyright (c) 1996,1997
               : * Silicon Graphics Computer Systems, Inc.
               : *
               : * Permission to use, copy, modify, distribute and sell this software
               : * and its documentation for any purpose is hereby granted without fee,
               : * provided that the above copyright notice appear in all copies and
               : * that both that copyright notice and this permission notice appear
               : * in supporting documentation.  Silicon Graphics makes no
               : * representations about the suitability of this software for any
               : * purpose.  It is provided "as is" without express or implied warranty.
               : */
               :
               :/** @file stl_list.h
               : *  This is an internal header file, included by other library headers.
               : *  You should not attempt to use it directly.
               : */
               :
               :#ifndef _STL_LIST_H
               :#define _STL_LIST_H 1
               :
               :#include <bits/concept_check.h>
               :
               :_GLIBCXX_BEGIN_NESTED_NAMESPACE(std, _GLIBCXX_STD_D)
               :
               :  // Supporting structures are split into common and templated types; the
               :  // latter publicly inherits from the former in an effort to reduce code
               :  // duplication.  This results in some "needless" static_cast'ing later on,
               :  // but it's all safe downcasting.
               :
               :  /// Common part of a node in the %list. 
               :  struct _List_node_base
               :  {
               :    _List_node_base* _M_next;
               :    _List_node_base* _M_prev;
               :
               :    static void
               :    swap(_List_node_base& __x, _List_node_base& __y);
               :
               :    void
               :    transfer(_List_node_base * const __first,
               :	     _List_node_base * const __last);
               :
               :    void
               :    reverse();
               :
               :    void
               :    hook(_List_node_base * const __position);
               :
               :    void
               :    unhook();
               :  };
               :
               :  /// An actual node in the %list.
               :  template<typename _Tp>
               :    struct _List_node : public _List_node_base
               :    {
               :      ///< User's data.
               :      _Tp _M_data;
               :    };
               :
               :  /**
               :   *  @brief A list::iterator.
               :   *
               :   *  All the functions are op overloads.
               :  */
               :  template<typename _Tp>
               :    struct _List_iterator
               :    {
               :      typedef _List_iterator<_Tp>                _Self;
               :      typedef _List_node<_Tp>                    _Node;
               :
               :      typedef ptrdiff_t                          difference_type;
               :      typedef std::bidirectional_iterator_tag    iterator_category;
               :      typedef _Tp                                value_type;
               :      typedef _Tp*                               pointer;
               :      typedef _Tp&                               reference;
               :
               :      _List_iterator()
               :      : _M_node() { }
               :
               :      explicit
  2454  0.4228 :      _List_iterator(_List_node_base* __x) /* std::_List_iterator<Gen*>::_List_iterator(std::_List_node_base*)    425  0.0732, std::_List_iterator<Command*>::_List_iterator(std::_List_node_base*)   2182  0.3760, std::_List_iterator<Obstacle*>::_List_iterator(std::_List_node_base*)   2060  0.3549, std::_List_iterator<Creature*>::_List_iterator(std::_List_node_base*)   1276  0.2199, total:   5943  1.0240 */
  3489  0.6012 :      : _M_node(__x) { }
               :
               :      // Must downcast from List_node_base to _List_node to get to _M_data.
               :      reference
  9769  1.6833 :      operator*() const /* std::_List_iterator<Gen*>::operator*() const    201  0.0346, std::_List_iterator<Command*>::operator*() const   1333  0.2297, std::_List_iterator<Obstacle*>::operator*() const  19627  3.3818, std::_List_iterator<Creature*>::operator*() const    872  0.1503, total:  22033  3.7964 */
 12264  2.1132 :      { return static_cast<_Node*>(_M_node)->_M_data; }
               :
               :      pointer
               :      operator->() const
               :      { return &static_cast<_Node*>(_M_node)->_M_data; }
               :
               :      _Self&
  9746  1.6793 :      operator++() /* std::_List_iterator<Gen*>::operator++()    763  0.1315, std::_List_iterator<Command*>::operator++()   1367  0.2355, std::_List_iterator<Obstacle*>::operator++()  27830  4.7953, std::_List_iterator<Creature*>::operator++()    815  0.1404, total:  30775  5.3027 */
               :      {
 12864  2.2165 :	_M_node = _M_node->_M_next;
   584  0.1006 :	return *this;
  7581  1.3062 :      }
               :
               :      _Self
               :      operator++(int)
               :      {
               :	_Self __tmp = *this;
               :	_M_node = _M_node->_M_next;
               :	return __tmp;
               :      }
               :
               :      _Self&
               :      operator--()
               :      {
               :	_M_node = _M_node->_M_prev;
               :	return *this;
               :      }
               :
               :      _Self
               :      operator--(int)
               :      {
               :	_Self __tmp = *this;
               :	_M_node = _M_node->_M_prev;
               :	return __tmp;
               :      }
               :
               :      bool
               :      operator==(const _Self& __x) const
               :      { return _M_node == __x._M_node; }
               :
               :      bool
 10820  1.8643 :      operator!=(const _Self& __x) const /* std::_List_iterator<Gen*>::operator!=(std::_List_iterator<Gen*> const&) const    531  0.0915, std::_List_iterator<Command*>::operator!=(std::_List_iterator<Command*> const&) const   3069  0.5288, std::_List_iterator<Obstacle*>::operator!=(std::_List_iterator<Obstacle*> const&) const  48639  8.3808, std::_List_iterator<Creature*>::operator!=(std::_List_iterator<Creature*> const&) const   2306  0.3973, total:  54545  9.3984 */
 43725  7.5341 :      { return _M_node != __x._M_node; }
               :
               :      // The only member points to the %list element.
               :      _List_node_base* _M_node;
               :    };
               :
               :  /**
               :   *  @brief A list::const_iterator.
               :   *
               :   *  All the functions are op overloads.
               :  */
               :  template<typename _Tp>
               :    struct _List_const_iterator
               :    {
               :      typedef _List_const_iterator<_Tp>          _Self;
               :      typedef const _List_node<_Tp>              _Node;
               :      typedef _List_iterator<_Tp>                iterator;
               :
               :      typedef ptrdiff_t                          difference_type;
               :      typedef std::bidirectional_iterator_tag    iterator_category;
               :      typedef _Tp                                value_type;
               :      typedef const _Tp*                         pointer;
               :      typedef const _Tp&                         reference;
               :
               :      _List_const_iterator()
               :      : _M_node() { }
               :
               :      explicit
   144  0.0248 :      _List_const_iterator(const _List_node_base* __x) /* std::_List_const_iterator<Obstacle*>::_List_const_iterator(std::_List_node_base const*)    198  0.0341, std::_List_const_iterator<Creature*>::_List_const_iterator(std::_List_node_base const*)    207  0.0357, total:    405  0.0698 */
   261  0.0450 :      : _M_node(__x) { }
               :
               :      _List_const_iterator(const iterator& __x)
               :      : _M_node(__x._M_node) { }
               :
               :      // Must downcast from List_node_base to _List_node to get to
               :      // _M_data.
               :      reference
               :      operator*() const
               :      { return static_cast<_Node*>(_M_node)->_M_data; }
               :
               :      pointer
               :      operator->() const
               :      { return &static_cast<_Node*>(_M_node)->_M_data; }
               :
               :      _Self&
   517  0.0891 :      operator++() /* std::_List_const_iterator<Obstacle*>::operator++()   1683  0.2900, std::_List_const_iterator<Creature*>::operator++()    145  0.0250, total:   1828  0.3150 */
               :      {
   868  0.1496 :	_M_node = _M_node->_M_next;
    65  0.0112 :	return *this;
   378  0.0651 :      }
               :
               :      _Self
               :      operator++(int)
               :      {
               :	_Self __tmp = *this;
               :	_M_node = _M_node->_M_next;
               :	return __tmp;
               :      }
               :
               :      _Self&
               :      operator--()
               :      {
               :	_M_node = _M_node->_M_prev;
               :	return *this;
               :      }
               :
               :      _Self
               :      operator--(int)
               :      {
               :	_Self __tmp = *this;
               :	_M_node = _M_node->_M_prev;
               :	return __tmp;
               :      }
               :
               :      bool
               :      operator==(const _Self& __x) const
               :      { return _M_node == __x._M_node; }
               :
               :      bool
   754  0.1299 :      operator!=(const _Self& __x) const /* std::_List_const_iterator<Obstacle*>::operator!=(std::_List_const_iterator<Obstacle*> const&) const   2768  0.4769, std::_List_const_iterator<Creature*>::operator!=(std::_List_const_iterator<Creature*> const&) const    333  0.0574, total:   3101  0.5343 */
  2347  0.4044 :      { return _M_node != __x._M_node; }
               :
               :      // The only member points to the %list element.
               :      const _List_node_base* _M_node;
               :    };
               :
               :  template<typename _Val>
               :    inline bool
               :    operator==(const _List_iterator<_Val>& __x,
               :	       const _List_const_iterator<_Val>& __y)
               :    { return __x._M_node == __y._M_node; }
               :
               :  template<typename _Val>
               :    inline bool
               :    operator!=(const _List_iterator<_Val>& __x,
               :               const _List_const_iterator<_Val>& __y)
               :    { return __x._M_node != __y._M_node; }
               :
               :
               :  /// See bits/stl_deque.h's _Deque_base for an explanation.
               :  template<typename _Tp, typename _Alloc>
               :    class _List_base
               :    {
               :    protected:
               :      // NOTA BENE
               :      // The stored instance is not actually of "allocator_type"'s
               :      // type.  Instead we rebind the type to
               :      // Allocator<List_node<Tp>>, which according to [20.1.5]/4
               :      // should probably be the same.  List_node<Tp> is not the same
               :      // size as Tp (it's two pointers larger), and specializations on
               :      // Tp may go unused because List_node<Tp> is being bound
               :      // instead.
               :      //
               :      // We put this to the test in the constructors and in
               :      // get_allocator, where we use conversions between
               :      // allocator_type and _Node_alloc_type. The conversion is
               :      // required by table 32 in [20.1.5].
               :      typedef typename _Alloc::template rebind<_List_node<_Tp> >::other
               :        _Node_alloc_type;
               :
               :      typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;
               :
               :      struct _List_impl 
               :      : public _Node_alloc_type
               :      {
               :	_List_node_base _M_node;
               :
               :	_List_impl() /* std::_List_base<Command*, std::allocator<Command*> >::_List_impl::_List_impl() total:      1 1.7e-04 */
     1 1.7e-04 :	: _Node_alloc_type(), _M_node()
               :	{ }
               :
               :	_List_impl(const _Node_alloc_type& __a)
               :	: _Node_alloc_type(__a), _M_node()
               :	{ }
               :      };
               :
               :      _List_impl _M_impl;
               :
               :      _List_node<_Tp>*
    37  0.0064 :      _M_get_node() /* std::_List_base<Command*, std::allocator<Command*> >::_M_get_node()      1 1.7e-04, std::_List_base<Obstacle*, std::allocator<Obstacle*> >::_M_get_node()    116  0.0200, total:    117  0.0202 */
    80  0.0138 :      { return _M_impl._Node_alloc_type::allocate(1); }
               :      
               :      void
    33  0.0057 :      _M_put_node(_List_node<_Tp>* __p) /* std::_List_base<Command*, std::allocator<Command*> >::_M_put_node(std::_List_node<Command*>*)      1 1.7e-04, std::_List_base<Obstacle*, std::allocator<Obstacle*> >::_M_put_node(std::_List_node<Obstacle*>*)    112  0.0193, total:    113  0.0195 */
    80  0.0138 :      { _M_impl._Node_alloc_type::deallocate(__p, 1); }
               :      
               :  public:
               :      typedef _Alloc allocator_type;
               :
               :      _Node_alloc_type&
               :      _M_get_Node_allocator()
               :      { return *static_cast<_Node_alloc_type*>(&this->_M_impl); }
               :
               :      const _Node_alloc_type&
    44  0.0076 :      _M_get_Node_allocator() const /* std::_List_base<Obstacle*, std::allocator<Obstacle*> >::_M_get_Node_allocator() const total:     78  0.0134 */
    34  0.0059 :      { return *static_cast<const _Node_alloc_type*>(&this->_M_impl); }
               :
               :      _Tp_alloc_type
    75  0.0129 :      _M_get_Tp_allocator() const /* std::_List_base<Obstacle*, std::allocator<Obstacle*> >::_M_get_Tp_allocator() const total:    303  0.0522 */
   228  0.0393 :      { return _Tp_alloc_type(_M_get_Node_allocator()); }
               :
               :      allocator_type
               :      get_allocator() const
               :      { return allocator_type(_M_get_Node_allocator()); }
               :
     1 1.7e-04 :      _List_base() /* std::_List_base<Gen*, std::allocator<Gen*> >::_List_base()      1 1.7e-04, std::_List_base<Command*, std::allocator<Command*> >::_List_base()      1 1.7e-04, total:      2 3.4e-04 */
               :      : _M_impl()
     1 1.7e-04 :      { _M_init(); }
               :
               :      _List_base(const allocator_type& __a)
               :      : _M_impl(__a)
               :      { _M_init(); }
               :
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      _List_base(_List_base&& __x)
               :      : _M_impl(__x._M_get_Node_allocator())
               :      {
               :	_M_init();
               :	_List_node_base::swap(this->_M_impl._M_node, __x._M_impl._M_node);	
               :      }
               :#endif
               :
               :      // This is what actually destroys the list.
               :      ~_List_base()
               :      { _M_clear(); }
               :
               :      void
               :      _M_clear();
               :
               :      void
     1 1.7e-04 :      _M_init() /* std::_List_base<Obstacle*, std::allocator<Obstacle*> >::_M_init() total:      2 3.4e-04 */
               :      {
     1 1.7e-04 :        this->_M_impl._M_node._M_next = &this->_M_impl._M_node;
               :        this->_M_impl._M_node._M_prev = &this->_M_impl._M_node;
               :      }
               :    };
               :
               :  /**
               :   *  @brief A standard container with linear time access to elements,
               :   *  and fixed time insertion/deletion at any point in the sequence.
               :   *
               :   *  @ingroup Containers
               :   *  @ingroup Sequences
               :   *
               :   *  Meets the requirements of a <a href="tables.html#65">container</a>, a
               :   *  <a href="tables.html#66">reversible container</a>, and a
               :   *  <a href="tables.html#67">sequence</a>, including the
               :   *  <a href="tables.html#68">optional sequence requirements</a> with the
               :   *  %exception of @c at and @c operator[].
               :   *
               :   *  This is a @e doubly @e linked %list.  Traversal up and down the
               :   *  %list requires linear time, but adding and removing elements (or
               :   *  @e nodes) is done in constant time, regardless of where the
               :   *  change takes place.  Unlike std::vector and std::deque,
               :   *  random-access iterators are not provided, so subscripting ( @c
               :   *  [] ) access is not allowed.  For algorithms which only need
               :   *  sequential access, this lack makes no difference.
               :   *
               :   *  Also unlike the other standard containers, std::list provides
               :   *  specialized algorithms %unique to linked lists, such as
               :   *  splicing, sorting, and in-place reversal.
               :   *
               :   *  A couple points on memory allocation for list<Tp>:
               :   *
               :   *  First, we never actually allocate a Tp, we allocate
               :   *  List_node<Tp>'s and trust [20.1.5]/4 to DTRT.  This is to ensure
               :   *  that after elements from %list<X,Alloc1> are spliced into
               :   *  %list<X,Alloc2>, destroying the memory of the second %list is a
               :   *  valid operation, i.e., Alloc1 giveth and Alloc2 taketh away.
               :   *
               :   *  Second, a %list conceptually represented as
               :   *  @code
               :   *    A <---> B <---> C <---> D
               :   *  @endcode
               :   *  is actually circular; a link exists between A and D.  The %list
               :   *  class holds (as its only data member) a private list::iterator
               :   *  pointing to @e D, not to @e A!  To get to the head of the %list,
               :   *  we start at the tail and move forward by one.  When this member
               :   *  iterator's next/previous pointers refer to itself, the %list is
               :   *  %empty. 
               :  */
               :  template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
               :    class list : protected _List_base<_Tp, _Alloc>
               :    {
               :      // concept requirements
               :      typedef typename _Alloc::value_type                _Alloc_value_type;
               :      __glibcxx_class_requires(_Tp, _SGIAssignableConcept)
               :      __glibcxx_class_requires2(_Tp, _Alloc_value_type, _SameTypeConcept)
               :
               :      typedef _List_base<_Tp, _Alloc>                    _Base;
               :      typedef typename _Base::_Tp_alloc_type		 _Tp_alloc_type;
               :
               :    public:
               :      typedef _Tp                                        value_type;
               :      typedef typename _Tp_alloc_type::pointer           pointer;
               :      typedef typename _Tp_alloc_type::const_pointer     const_pointer;
               :      typedef typename _Tp_alloc_type::reference         reference;
               :      typedef typename _Tp_alloc_type::const_reference   const_reference;
               :      typedef _List_iterator<_Tp>                        iterator;
               :      typedef _List_const_iterator<_Tp>                  const_iterator;
               :      typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;
               :      typedef std::reverse_iterator<iterator>            reverse_iterator;
               :      typedef size_t                                     size_type;
               :      typedef ptrdiff_t                                  difference_type;
               :      typedef _Alloc                                     allocator_type;
               :
               :    protected:
               :      // Note that pointers-to-_Node's can be ctor-converted to
               :      // iterator types.
               :      typedef _List_node<_Tp>				 _Node;
               :
               :      using _Base::_M_impl;
               :      using _Base::_M_put_node;
               :      using _Base::_M_get_node;
               :      using _Base::_M_get_Tp_allocator;
               :      using _Base::_M_get_Node_allocator;
               :
               :      /**
               :       *  @param  x  An instance of user data.
               :       *
               :       *  Allocates space for a new node and constructs a copy of @a x in it.
               :       */
               :#ifndef __GXX_EXPERIMENTAL_CXX0X__
               :      _Node*
    36  0.0062 :      _M_create_node(const value_type& __x) /* std::list<Obstacle*, std::allocator<Obstacle*> >::_M_create_node(Obstacle* const&) total:    270  0.0465 */
               :      {
    40  0.0069 :	_Node* __p = this->_M_get_node();
               :	try
               :	  {
   132  0.0227 :	    _M_get_Tp_allocator().construct(&__p->_M_data, __x);
               :	  }
               :	catch(...)
               :	  {
               :	    _M_put_node(__p);
               :	    __throw_exception_again;
               :	  }
    23  0.0040 :	return __p;
    39  0.0067 :      }
               :#else
               :      template<typename... _Args>
               :        _Node*
               :        _M_create_node(_Args&&... __args)
               :	{
               :	  _Node* __p = this->_M_get_node();
               :	  try
               :	    {
               :	      _M_get_Tp_allocator().construct(&__p->_M_data,
               :					      std::forward<_Args>(__args)...);
               :	    }
               :	  catch(...)
               :	    {
               :	      _M_put_node(__p);
               :	      __throw_exception_again;
               :	    }
               :	  return __p;
               :	}
               :#endif
               :
               :    public:
               :      // [23.2.2.1] construct/copy/destroy
               :      // (assign() and get_allocator() are also listed in this section)
               :      /**
               :       *  @brief  Default constructor creates no elements.
               :       */
               :      list()
               :      : _Base() { }
               :
               :      /**
               :       *  @brief  Creates a %list with no elements.
               :       *  @param  a  An allocator object.
               :       */
               :      explicit
               :      list(const allocator_type& __a)
               :      : _Base(__a) { }
               :
               :      /**
               :       *  @brief  Creates a %list with copies of an exemplar element.
               :       *  @param  n  The number of elements to initially create.
               :       *  @param  value  An element to copy.
               :       *  @param  a  An allocator object.
               :       *
               :       *  This constructor fills the %list with @a n copies of @a value.
               :       */
               :      explicit
               :      list(size_type __n, const value_type& __value = value_type(),
               :	   const allocator_type& __a = allocator_type())
               :      : _Base(__a)
               :      { _M_fill_initialize(__n, __value); }
               :
               :      /**
               :       *  @brief  %List copy constructor.
               :       *  @param  x  A %list of identical element and allocator types.
               :       *
               :       *  The newly-created %list uses a copy of the allocation object used
               :       *  by @a x.
               :       */
               :      list(const list& __x)
               :      : _Base(__x._M_get_Node_allocator())
               :      { _M_initialize_dispatch(__x.begin(), __x.end(), __false_type()); }
               :
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      /**
               :       *  @brief  %List move constructor.
               :       *  @param  x  A %list of identical element and allocator types.
               :       *
               :       *  The newly-created %list contains the exact contents of @a x.
               :       *  The contents of @a x are a valid, but unspecified %list.
               :       */
               :      list(list&& __x)
               :      : _Base(std::forward<_Base>(__x)) { }
               :#endif
               :
               :      /**
               :       *  @brief  Builds a %list from a range.
               :       *  @param  first  An input iterator.
               :       *  @param  last  An input iterator.
               :       *  @param  a  An allocator object.
               :       *
               :       *  Create a %list consisting of copies of the elements from
               :       *  [@a first,@a last).  This is linear in N (where N is
               :       *  distance(@a first,@a last)).
               :       */
               :      template<typename _InputIterator>
               :        list(_InputIterator __first, _InputIterator __last,
               :	     const allocator_type& __a = allocator_type())
               :        : _Base(__a)
               :        { 
               :	  // Check whether it's an integral type.  If so, it's not an iterator.
               :	  typedef typename std::__is_integer<_InputIterator>::__type _Integral;
               :	  _M_initialize_dispatch(__first, __last, _Integral());
               :	}
               :
               :      /**
               :       *  No explicit dtor needed as the _Base dtor takes care of
               :       *  things.  The _Base dtor only erases the elements, and note
               :       *  that if the elements themselves are pointers, the pointed-to
               :       *  memory is not touched in any way.  Managing the pointer is
               :       *  the user's responsibility.
               :       */
               :
               :      /**
               :       *  @brief  %List assignment operator.
               :       *  @param  x  A %list of identical element and allocator types.
               :       *
               :       *  All the elements of @a x are copied, but unlike the copy
               :       *  constructor, the allocator object is not copied.
               :       */
               :      list&
               :      operator=(const list& __x);
               :
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      /**
               :       *  @brief  %List move assignment operator.
               :       *  @param  x  A %list of identical element and allocator types.
               :       *
               :       *  The contents of @a x are moved into this %list (without copying).
               :       *  @a x is a valid, but unspecified %list
               :       */
               :      list&
               :      operator=(list&& __x)
               :      {
               :	// NB: DR 675.
               :	this->clear();
               :	this->swap(__x); 
               :	return *this;
               :      }
               :#endif
               :
               :      /**
               :       *  @brief  Assigns a given value to a %list.
               :       *  @param  n  Number of elements to be assigned.
               :       *  @param  val  Value to be assigned.
               :       *
               :       *  This function fills a %list with @a n copies of the given
               :       *  value.  Note that the assignment completely changes the %list
               :       *  and that the resulting %list's size is the same as the number
               :       *  of elements assigned.  Old data may be lost.
               :       */
               :      void
               :      assign(size_type __n, const value_type& __val)
               :      { _M_fill_assign(__n, __val); }
               :
               :      /**
               :       *  @brief  Assigns a range to a %list.
               :       *  @param  first  An input iterator.
               :       *  @param  last   An input iterator.
               :       *
               :       *  This function fills a %list with copies of the elements in the
               :       *  range [@a first,@a last).
               :       *
               :       *  Note that the assignment completely changes the %list and
               :       *  that the resulting %list's size is the same as the number of
               :       *  elements assigned.  Old data may be lost.
               :       */
               :      template<typename _InputIterator>
               :        void
               :        assign(_InputIterator __first, _InputIterator __last)
               :        {
               :	  // Check whether it's an integral type.  If so, it's not an iterator.
               :	  typedef typename std::__is_integer<_InputIterator>::__type _Integral;
               :	  _M_assign_dispatch(__first, __last, _Integral());
               :	}
               :
               :      /// Get a copy of the memory allocation object.
               :      allocator_type
               :      get_allocator() const
               :      { return _Base::get_allocator(); }
               :
               :      // iterators
               :      /**
               :       *  Returns a read/write iterator that points to the first element in the
               :       *  %list.  Iteration is done in ordinary element order.
               :       */
               :      iterator
  1259  0.2169 :      begin() /* std::list<Gen*, std::allocator<Gen*> >::begin()    232  0.0400, std::list<Command*, std::allocator<Command*> >::begin()   1351  0.2328, std::list<Obstacle*, std::allocator<Obstacle*> >::begin()   1347  0.2321, std::list<Creature*, std::allocator<Creature*> >::begin()   1213  0.2090, total:   4143  0.7139 */
  2884  0.4969 :      { return iterator(this->_M_impl._M_node._M_next); }
               :
               :      /**
               :       *  Returns a read-only (constant) iterator that points to the
               :       *  first element in the %list.  Iteration is done in ordinary
               :       *  element order.
               :       */
               :      const_iterator
   129  0.0222 :      begin() const /* std::list<Gen*, std::allocator<Gen*> >::begin() const      1 1.7e-04, std::list<Obstacle*, std::allocator<Obstacle*> >::begin() const    119  0.0205, std::list<Creature*, std::allocator<Creature*> >::begin() const    303  0.0522, total:    423  0.0729 */
   294  0.0507 :      { return const_iterator(this->_M_impl._M_node._M_next); }
               :
               :      /**
               :       *  Returns a read/write iterator that points one past the last
               :       *  element in the %list.  Iteration is done in ordinary element
               :       *  order.
               :       */
               :      iterator
  1963  0.3382 :      end() /* std::list<Gen*, std::allocator<Gen*> >::end()    453  0.0781, std::list<Command*, std::allocator<Command*> >::end()   2439  0.4203, std::list<Obstacle*, std::allocator<Obstacle*> >::end()   1716  0.2957, std::list<Creature*, std::allocator<Creature*> >::end()    885  0.1525, total:   5493  0.9465 */
  3530  0.6082 :      { return iterator(&this->_M_impl._M_node); }
               :
               :      /**
               :       *  Returns a read-only (constant) iterator that points one past
               :       *  the last element in the %list.  Iteration is done in ordinary
               :       *  element order.
               :       */
               :      const_iterator
   143  0.0246 :      end() const /* std::list<Command*, std::allocator<Command*> >::end() const      1 1.7e-04, std::list<Obstacle*, std::allocator<Obstacle*> >::end() const     77  0.0133, std::list<Creature*, std::allocator<Creature*> >::end() const    256  0.0441, total:    334  0.0576 */
   191  0.0329 :      { return const_iterator(&this->_M_impl._M_node); }
               :
               :      /**
               :       *  Returns a read/write reverse iterator that points to the last
               :       *  element in the %list.  Iteration is done in reverse element
               :       *  order.
               :       */
               :      reverse_iterator
               :      rbegin()
               :      { return reverse_iterator(end()); }
               :
               :      /**
               :       *  Returns a read-only (constant) reverse iterator that points to
               :       *  the last element in the %list.  Iteration is done in reverse
               :       *  element order.
               :       */
               :      const_reverse_iterator
               :      rbegin() const
               :      { return const_reverse_iterator(end()); }
               :
               :      /**
               :       *  Returns a read/write reverse iterator that points to one
               :       *  before the first element in the %list.  Iteration is done in
               :       *  reverse element order.
               :       */
               :      reverse_iterator
               :      rend()
               :      { return reverse_iterator(begin()); }
               :
               :      /**
               :       *  Returns a read-only (constant) reverse iterator that points to one
               :       *  before the first element in the %list.  Iteration is done in reverse
               :       *  element order.
               :       */
               :      const_reverse_iterator
               :      rend() const
               :      { return const_reverse_iterator(begin()); }
               :
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      /**
               :       *  Returns a read-only (constant) iterator that points to the
               :       *  first element in the %list.  Iteration is done in ordinary
               :       *  element order.
               :       */
               :      const_iterator
               :      cbegin() const
               :      { return const_iterator(this->_M_impl._M_node._M_next); }
               :
               :      /**
               :       *  Returns a read-only (constant) iterator that points one past
               :       *  the last element in the %list.  Iteration is done in ordinary
               :       *  element order.
               :       */
               :      const_iterator
               :      cend() const
               :      { return const_iterator(&this->_M_impl._M_node); }
               :
               :      /**
               :       *  Returns a read-only (constant) reverse iterator that points to
               :       *  the last element in the %list.  Iteration is done in reverse
               :       *  element order.
               :       */
               :      const_reverse_iterator
               :      crbegin() const
               :      { return const_reverse_iterator(end()); }
               :
               :      /**
               :       *  Returns a read-only (constant) reverse iterator that points to one
               :       *  before the first element in the %list.  Iteration is done in reverse
               :       *  element order.
               :       */
               :      const_reverse_iterator
               :      crend() const
               :      { return const_reverse_iterator(begin()); }
               :#endif
               :
               :      // [23.2.2.2] capacity
               :      /**
               :       *  Returns true if the %list is empty.  (Thus begin() would equal
               :       *  end().)
               :       */
               :      bool
               :      empty() const
               :      { return this->_M_impl._M_node._M_next == &this->_M_impl._M_node; }
               :
               :      /**  Returns the number of elements in the %list.  */
               :      size_type
   401  0.0691 :      size() const /* std::list<Command*, std::allocator<Command*> >::size() const      1 1.7e-04, std::list<Obstacle*, std::allocator<Obstacle*> >::size() const    198  0.0341, std::list<Creature*, std::allocator<Creature*> >::size() const    859  0.1480, total:   1058  0.1823 */
   657  0.1132 :      { return std::distance(begin(), end()); }
               :
               :      /**  Returns the size() of the largest possible %list.  */
               :      size_type
               :      max_size() const
               :      { return _M_get_Tp_allocator().max_size(); }
               :
               :      /**
               :       *  @brief Resizes the %list to the specified number of elements.
               :       *  @param new_size Number of elements the %list should contain.
               :       *  @param x Data with which new elements should be populated.
               :       *
               :       *  This function will %resize the %list to the specified number
               :       *  of elements.  If the number is smaller than the %list's
               :       *  current size the %list is truncated, otherwise the %list is
               :       *  extended and new elements are populated with given data.
               :       */
               :      void
               :      resize(size_type __new_size, value_type __x = value_type());
               :
               :      // element access
               :      /**
               :       *  Returns a read/write reference to the data at the first
               :       *  element of the %list.
               :       */
               :      reference
               :      front()
               :      { return *begin(); }
               :
               :      /**
               :       *  Returns a read-only (constant) reference to the data at the first
               :       *  element of the %list.
               :       */
               :      const_reference
               :      front() const
               :      { return *begin(); }
               :
               :      /**
               :       *  Returns a read/write reference to the data at the last element
               :       *  of the %list.
               :       */
               :      reference
               :      back()
               :      { 
               :	iterator __tmp = end();
               :	--__tmp;
               :	return *__tmp;
               :      }
               :
               :      /**
               :       *  Returns a read-only (constant) reference to the data at the last
               :       *  element of the %list.
               :       */
               :      const_reference
               :      back() const
               :      { 
               :	const_iterator __tmp = end();
               :	--__tmp;
               :	return *__tmp;
               :      }
               :
               :      // [23.2.2.3] modifiers
               :      /**
               :       *  @brief  Add data to the front of the %list.
               :       *  @param  x  Data to be added.
               :       *
               :       *  This is a typical stack operation.  The function creates an
               :       *  element at the front of the %list and assigns the given data
               :       *  to it.  Due to the nature of a %list this operation can be
               :       *  done in constant time, and does not invalidate iterators and
               :       *  references.
               :       */
               :#ifndef __GXX_EXPERIMENTAL_CXX0X__
               :      void
               :      push_front(const value_type& __x)
               :      { this->_M_insert(begin(), __x); }
               :#else
               :      template<typename... _Args>
               :        void
               :        push_front(_Args&&... __args)
               :	{ this->_M_insert(begin(), std::forward<_Args>(__args)...); }
               :#endif
               :
               :      /**
               :       *  @brief  Removes first element.
               :       *
               :       *  This is a typical stack operation.  It shrinks the %list by
               :       *  one.  Due to the nature of a %list this operation can be done
               :       *  in constant time, and only invalidates iterators/references to
               :       *  the element being removed.
               :       *
               :       *  Note that no data is returned, and if the first element's data
               :       *  is needed, it should be retrieved before pop_front() is
               :       *  called.
               :       */
               :      void
               :      pop_front()
               :      { this->_M_erase(begin()); }
               :
               :      /**
               :       *  @brief  Add data to the end of the %list.
               :       *  @param  x  Data to be added.
               :       *
               :       *  This is a typical stack operation.  The function creates an
               :       *  element at the end of the %list and assigns the given data to
               :       *  it.  Due to the nature of a %list this operation can be done
               :       *  in constant time, and does not invalidate iterators and
               :       *  references.
               :       */
               :#ifndef __GXX_EXPERIMENTAL_CXX0X__
               :      void
    22  0.0038 :      push_back(const value_type& __x) /* std::list<Command*, std::allocator<Command*> >::push_back(Command* const&)      1 1.7e-04, std::list<Obstacle*, std::allocator<Obstacle*> >::push_back(Obstacle* const&)    140  0.0241, total:    141  0.0243 */
   119  0.0205 :      { this->_M_insert(end(), __x); }
               :#else
               :      template<typename... _Args>
               :        void
               :        push_back(_Args&&... __args)
               :	{ this->_M_insert(end(), std::forward<_Args>(__args)...); }
               :#endif
               :
               :      /**
               :       *  @brief  Removes last element.
               :       *
               :       *  This is a typical stack operation.  It shrinks the %list by
               :       *  one.  Due to the nature of a %list this operation can be done
               :       *  in constant time, and only invalidates iterators/references to
               :       *  the element being removed.
               :       *
               :       *  Note that no data is returned, and if the last element's data
               :       *  is needed, it should be retrieved before pop_back() is called.
               :       */
               :      void
               :      pop_back()
               :      { this->_M_erase(iterator(this->_M_impl._M_node._M_prev)); }
               :
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      /**
               :       *  @brief  Constructs object in %list before specified iterator.
               :       *  @param  position  A const_iterator into the %list.
               :       *  @param  args  Arguments.
               :       *  @return  An iterator that points to the inserted data.
               :       *
               :       *  This function will insert an object of type T constructed
               :       *  with T(std::forward<Args>(args)...) before the specified
               :       *  location.  Due to the nature of a %list this operation can
               :       *  be done in constant time, and does not invalidate iterators
               :       *  and references.
               :       */
               :      template<typename... _Args>
               :        iterator
               :        emplace(iterator __position, _Args&&... __args);
               :#endif
               :
               :      /**
               :       *  @brief  Inserts given value into %list before specified iterator.
               :       *  @param  position  An iterator into the %list.
               :       *  @param  x  Data to be inserted.
               :       *  @return  An iterator that points to the inserted data.
               :       *
               :       *  This function will insert a copy of the given value before
               :       *  the specified location.  Due to the nature of a %list this
               :       *  operation can be done in constant time, and does not
               :       *  invalidate iterators and references.
               :       */
               :      iterator
               :      insert(iterator __position, const value_type& __x);
               :
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      /**
               :       *  @brief  Inserts given rvalue into %list before specified iterator.
               :       *  @param  position  An iterator into the %list.
               :       *  @param  x  Data to be inserted.
               :       *  @return  An iterator that points to the inserted data.
               :       *
               :       *  This function will insert a copy of the given rvalue before
               :       *  the specified location.  Due to the nature of a %list this
               :       *  operation can be done in constant time, and does not
               :       *  invalidate iterators and references.
               :        */
               :      iterator
               :      insert(iterator __position, value_type&& __x)
               :      { return emplace(__position, std::move(__x)); }
               :#endif
               :
               :      /**
               :       *  @brief  Inserts a number of copies of given data into the %list.
               :       *  @param  position  An iterator into the %list.
               :       *  @param  n  Number of elements to be inserted.
               :       *  @param  x  Data to be inserted.
               :       *
               :       *  This function will insert a specified number of copies of the
               :       *  given data before the location specified by @a position.
               :       *
               :       *  This operation is linear in the number of elements inserted and
               :       *  does not invalidate iterators and references.
               :       */
               :      void
               :      insert(iterator __position, size_type __n, const value_type& __x)
               :      {  
               :	list __tmp(__n, __x, _M_get_Node_allocator());
               :	splice(__position, __tmp);
               :      }
               :
               :      /**
               :       *  @brief  Inserts a range into the %list.
               :       *  @param  position  An iterator into the %list.
               :       *  @param  first  An input iterator.
               :       *  @param  last   An input iterator.
               :       *
               :       *  This function will insert copies of the data in the range [@a
               :       *  first,@a last) into the %list before the location specified by
               :       *  @a position.
               :       *
               :       *  This operation is linear in the number of elements inserted and
               :       *  does not invalidate iterators and references.
               :       */
               :      template<typename _InputIterator>
               :        void
               :        insert(iterator __position, _InputIterator __first,
               :	       _InputIterator __last)
               :        {
               :	  list __tmp(__first, __last, _M_get_Node_allocator());
               :	  splice(__position, __tmp);
               :	}
               :
               :      /**
               :       *  @brief  Remove element at given position.
               :       *  @param  position  Iterator pointing to element to be erased.
               :       *  @return  An iterator pointing to the next element (or end()).
               :       *
               :       *  This function will erase the element at the given position and thus
               :       *  shorten the %list by one.
               :       *
               :       *  Due to the nature of a %list this operation can be done in
               :       *  constant time, and only invalidates iterators/references to
               :       *  the element being removed.  The user is also cautioned that
               :       *  this function only erases the element, and that if the element
               :       *  is itself a pointer, the pointed-to memory is not touched in
               :       *  any way.  Managing the pointer is the user's responsibility.
               :       */
               :      iterator
               :      erase(iterator __position);
               :
               :      /**
               :       *  @brief  Remove a range of elements.
               :       *  @param  first  Iterator pointing to the first element to be erased.
               :       *  @param  last  Iterator pointing to one past the last element to be
               :       *                erased.
               :       *  @return  An iterator pointing to the element pointed to by @a last
               :       *           prior to erasing (or end()).
               :       *
               :       *  This function will erase the elements in the range @a
               :       *  [first,last) and shorten the %list accordingly.
               :       *
               :       *  This operation is linear time in the size of the range and only
               :       *  invalidates iterators/references to the element being removed.
               :       *  The user is also cautioned that this function only erases the
               :       *  elements, and that if the elements themselves are pointers, the
               :       *  pointed-to memory is not touched in any way.  Managing the pointer
               :       *  is the user's responsibility.
               :       */
               :      iterator
               :      erase(iterator __first, iterator __last)
               :      {
               :	while (__first != __last)
               :	  __first = erase(__first);
               :	return __last;
               :      }
               :
               :      /**
               :       *  @brief  Swaps data with another %list.
               :       *  @param  x  A %list of the same element and allocator types.
               :       *
               :       *  This exchanges the elements between two lists in constant
               :       *  time.  Note that the global std::swap() function is
               :       *  specialized such that std::swap(l1,l2) will feed to this
               :       *  function.
               :       */
               :      void
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      swap(list&& __x)
               :#else
               :      swap(list& __x)
               :#endif
               :      {
               :	_List_node_base::swap(this->_M_impl._M_node, __x._M_impl._M_node);
               :
               :	// _GLIBCXX_RESOLVE_LIB_DEFECTS
               :	// 431. Swapping containers with unequal allocators.
               :	std::__alloc_swap<typename _Base::_Node_alloc_type>::
               :	  _S_do_it(_M_get_Node_allocator(), __x._M_get_Node_allocator());
               :      }
               :
               :      /**
               :       *  Erases all the elements.  Note that this function only erases
               :       *  the elements, and that if the elements themselves are
               :       *  pointers, the pointed-to memory is not touched in any way.
               :       *  Managing the pointer is the user's responsibility.
               :       */
               :      void
    10  0.0017 :      clear() /* std::list<Command*, std::allocator<Command*> >::clear()      1 1.7e-04, std::list<Obstacle*, std::allocator<Obstacle*> >::clear()     12  0.0021, total:     13  0.0022 */
               :      {
               :        _Base::_M_clear();
     3 5.2e-04 :        _Base::_M_init();
               :      }
               :
               :      // [23.2.2.4] list operations
               :      /**
               :       *  @brief  Insert contents of another %list.
               :       *  @param  position  Iterator referencing the element to insert before.
               :       *  @param  x  Source list.
               :       *
               :       *  The elements of @a x are inserted in constant time in front of
               :       *  the element referenced by @a position.  @a x becomes an empty
               :       *  list.
               :       *
               :       *  Requires this != @a x.
               :       */
               :      void
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      splice(iterator __position, list&& __x)
               :#else
               :      splice(iterator __position, list& __x)
               :#endif
               :      {
               :	if (!__x.empty())
               :	  {
               :	    _M_check_equal_allocators(__x);
               :
               :	    this->_M_transfer(__position, __x.begin(), __x.end());
               :	  }
               :      }
               :
               :      /**
               :       *  @brief  Insert element from another %list.
               :       *  @param  position  Iterator referencing the element to insert before.
               :       *  @param  x  Source list.
               :       *  @param  i  Iterator referencing the element to move.
               :       *
               :       *  Removes the element in list @a x referenced by @a i and
               :       *  inserts it into the current list before @a position.
               :       */
               :      void
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      splice(iterator __position, list&& __x, iterator __i)
               :#else
               :      splice(iterator __position, list& __x, iterator __i)
               :#endif
               :      {
               :	iterator __j = __i;
               :	++__j;
               :	if (__position == __i || __position == __j)
               :	  return;
               :
               :	if (this != &__x)
               :	  _M_check_equal_allocators(__x);
               :
               :	this->_M_transfer(__position, __i, __j);
               :      }
               :
               :      /**
               :       *  @brief  Insert range from another %list.
               :       *  @param  position  Iterator referencing the element to insert before.
               :       *  @param  x  Source list.
               :       *  @param  first  Iterator referencing the start of range in x.
               :       *  @param  last  Iterator referencing the end of range in x.
               :       *
               :       *  Removes elements in the range [first,last) and inserts them
               :       *  before @a position in constant time.
               :       *
               :       *  Undefined if @a position is in [first,last).
               :       */
               :      void
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      splice(iterator __position, list&& __x, iterator __first,
               :	     iterator __last)
               :#else
               :      splice(iterator __position, list& __x, iterator __first,
               :	     iterator __last)
               :#endif
               :      {
               :	if (__first != __last)
               :	  {
               :	    if (this != &__x)
               :	      _M_check_equal_allocators(__x);
               :
               :	    this->_M_transfer(__position, __first, __last);
               :	  }
               :      }
               :
               :      /**
               :       *  @brief  Remove all elements equal to value.
               :       *  @param  value  The value to remove.
               :       *
               :       *  Removes every element in the list equal to @a value.
               :       *  Remaining elements stay in list order.  Note that this
               :       *  function only erases the elements, and that if the elements
               :       *  themselves are pointers, the pointed-to memory is not
               :       *  touched in any way.  Managing the pointer is the user's
               :       *  responsibility.
               :       */
               :      void
               :      remove(const _Tp& __value);
               :
               :      /**
               :       *  @brief  Remove all elements satisfying a predicate.
               :       *  @param  Predicate  Unary predicate function or object.
               :       *
               :       *  Removes every element in the list for which the predicate
               :       *  returns true.  Remaining elements stay in list order.  Note
               :       *  that this function only erases the elements, and that if the
               :       *  elements themselves are pointers, the pointed-to memory is
               :       *  not touched in any way.  Managing the pointer is the user's
               :       *  responsibility.
               :       */
               :      template<typename _Predicate>
               :        void
               :        remove_if(_Predicate);
               :
               :      /**
               :       *  @brief  Remove consecutive duplicate elements.
               :       *
               :       *  For each consecutive set of elements with the same value,
               :       *  remove all but the first one.  Remaining elements stay in
               :       *  list order.  Note that this function only erases the
               :       *  elements, and that if the elements themselves are pointers,
               :       *  the pointed-to memory is not touched in any way.  Managing
               :       *  the pointer is the user's responsibility.
               :       */
               :      void
               :      unique();
               :
               :      /**
               :       *  @brief  Remove consecutive elements satisfying a predicate.
               :       *  @param  BinaryPredicate  Binary predicate function or object.
               :       *
               :       *  For each consecutive set of elements [first,last) that
               :       *  satisfy predicate(first,i) where i is an iterator in
               :       *  [first,last), remove all but the first one.  Remaining
               :       *  elements stay in list order.  Note that this function only
               :       *  erases the elements, and that if the elements themselves are
               :       *  pointers, the pointed-to memory is not touched in any way.
               :       *  Managing the pointer is the user's responsibility.
               :       */
               :      template<typename _BinaryPredicate>
               :        void
               :        unique(_BinaryPredicate);
               :
               :      /**
               :       *  @brief  Merge sorted lists.
               :       *  @param  x  Sorted list to merge.
               :       *
               :       *  Assumes that both @a x and this list are sorted according to
               :       *  operator<().  Merges elements of @a x into this list in
               :       *  sorted order, leaving @a x empty when complete.  Elements in
               :       *  this list precede elements in @a x that are equal.
               :       */
               :      void
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :      merge(list&& __x);
               :#else
               :      merge(list& __x);
               :#endif
               :
               :      /**
               :       *  @brief  Merge sorted lists according to comparison function.
               :       *  @param  x  Sorted list to merge.
               :       *  @param StrictWeakOrdering Comparison function defining
               :       *  sort order.
               :       *
               :       *  Assumes that both @a x and this list are sorted according to
               :       *  StrictWeakOrdering.  Merges elements of @a x into this list
               :       *  in sorted order, leaving @a x empty when complete.  Elements
               :       *  in this list precede elements in @a x that are equivalent
               :       *  according to StrictWeakOrdering().
               :       */
               :      template<typename _StrictWeakOrdering>
               :        void
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :        merge(list&&, _StrictWeakOrdering);
               :#else
               :        merge(list&, _StrictWeakOrdering);
               :#endif
               :
               :      /**
               :       *  @brief  Reverse the elements in list.
               :       *
               :       *  Reverse the order of elements in the list in linear time.
               :       */
               :      void
               :      reverse()
               :      { this->_M_impl._M_node.reverse(); }
               :
               :      /**
               :       *  @brief  Sort the elements.
               :       *
               :       *  Sorts the elements of this list in NlogN time.  Equivalent
               :       *  elements remain in list order.
               :       */
               :      void
               :      sort();
               :
               :      /**
               :       *  @brief  Sort the elements according to comparison function.
               :       *
               :       *  Sorts the elements of this list in NlogN time.  Equivalent
               :       *  elements remain in list order.
               :       */
               :      template<typename _StrictWeakOrdering>
               :        void
               :        sort(_StrictWeakOrdering);
               :
               :    protected:
               :      // Internal constructor functions follow.
               :
               :      // Called by the range constructor to implement [23.1.1]/9
               :
               :      // _GLIBCXX_RESOLVE_LIB_DEFECTS
               :      // 438. Ambiguity in the "do the right thing" clause
               :      template<typename _Integer>
               :        void
               :        _M_initialize_dispatch(_Integer __n, _Integer __x, __true_type)
               :        { _M_fill_initialize(static_cast<size_type>(__n), __x); }
               :
               :      // Called by the range constructor to implement [23.1.1]/9
               :      template<typename _InputIterator>
               :        void
               :        _M_initialize_dispatch(_InputIterator __first, _InputIterator __last,
               :			       __false_type)
               :        {
               :	  for (; __first != __last; ++__first)
               :	    push_back(*__first);
               :	}
               :
               :      // Called by list(n,v,a), and the range constructor when it turns out
               :      // to be the same thing.
               :      void
               :      _M_fill_initialize(size_type __n, const value_type& __x)
               :      {
               :	for (; __n > 0; --__n)
               :	  push_back(__x);
               :      }
               :
               :
               :      // Internal assign functions follow.
               :
               :      // Called by the range assign to implement [23.1.1]/9
               :
               :      // _GLIBCXX_RESOLVE_LIB_DEFECTS
               :      // 438. Ambiguity in the "do the right thing" clause
               :      template<typename _Integer>
               :        void
               :        _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
               :        { _M_fill_assign(__n, __val); }
               :
               :      // Called by the range assign to implement [23.1.1]/9
               :      template<typename _InputIterator>
               :        void
               :        _M_assign_dispatch(_InputIterator __first, _InputIterator __last,
               :			   __false_type);
               :
               :      // Called by assign(n,t), and the range assign when it turns out
               :      // to be the same thing.
               :      void
               :      _M_fill_assign(size_type __n, const value_type& __val);
               :
               :
               :      // Moves the elements from [first,last) before position.
               :      void
               :      _M_transfer(iterator __position, iterator __first, iterator __last)
               :      { __position._M_node->transfer(__first._M_node, __last._M_node); }
               :
               :      // Inserts new element at position given and with value given.
               :#ifndef __GXX_EXPERIMENTAL_CXX0X__
               :      void
    19  0.0033 :      _M_insert(iterator __position, const value_type& __x) /* std::list<Gen*, std::allocator<Gen*> >::_M_insert(std::_List_iterator<Gen*>, Gen* const&)      1 1.7e-04, std::list<Obstacle*, std::allocator<Obstacle*> >::_M_insert(std::_List_iterator<Obstacle*>, Obstacle* const&)    200  0.0345, total:    201  0.0346 */
               :      {
    46  0.0079 :        _Node* __tmp = _M_create_node(__x);
    41  0.0071 :        __tmp->hook(__position._M_node);
    95  0.0164 :      }
               :#else
               :     template<typename... _Args>
               :       void
               :       _M_insert(iterator __position, _Args&&... __args)
               :       {
               :	 _Node* __tmp = _M_create_node(std::forward<_Args>(__args)...);
               :	 __tmp->hook(__position._M_node);
               :       }
               :#endif
               :
               :      // Erases element at position given.
               :      void
               :      _M_erase(iterator __position)
               :      {
               :        __position._M_node->unhook();
               :        _Node* __n = static_cast<_Node*>(__position._M_node);
               :        _M_get_Tp_allocator().destroy(&__n->_M_data);
               :        _M_put_node(__n);
               :      }
               :
               :      // To implement the splice (and merge) bits of N1599.
               :      void
               :      _M_check_equal_allocators(list& __x)
               :      {
               :	if (std::__alloc_neq<typename _Base::_Node_alloc_type>::
               :	    _S_do_it(_M_get_Node_allocator(), __x._M_get_Node_allocator()))
               :	  __throw_runtime_error(__N("list::_M_check_equal_allocators"));
               :      }
               :    };
               :
               :  /**
               :   *  @brief  List equality comparison.
               :   *  @param  x  A %list.
               :   *  @param  y  A %list of the same type as @a x.
               :   *  @return  True iff the size and elements of the lists are equal.
               :   *
               :   *  This is an equivalence relation.  It is linear in the size of
               :   *  the lists.  Lists are considered equivalent if their sizes are
               :   *  equal, and if corresponding elements compare equal.
               :  */
               :  template<typename _Tp, typename _Alloc>
               :    inline bool
               :    operator==(const list<_Tp, _Alloc>& __x, const list<_Tp, _Alloc>& __y)
               :    {
               :      typedef typename list<_Tp, _Alloc>::const_iterator const_iterator;
               :      const_iterator __end1 = __x.end();
               :      const_iterator __end2 = __y.end();
               :
               :      const_iterator __i1 = __x.begin();
               :      const_iterator __i2 = __y.begin();
               :      while (__i1 != __end1 && __i2 != __end2 && *__i1 == *__i2)
               :	{
               :	  ++__i1;
               :	  ++__i2;
               :	}
               :      return __i1 == __end1 && __i2 == __end2;
               :    }
               :
               :  /**
               :   *  @brief  List ordering relation.
               :   *  @param  x  A %list.
               :   *  @param  y  A %list of the same type as @a x.
               :   *  @return  True iff @a x is lexicographically less than @a y.
               :   *
               :   *  This is a total ordering relation.  It is linear in the size of the
               :   *  lists.  The elements must be comparable with @c <.
               :   *
               :   *  See std::lexicographical_compare() for how the determination is made.
               :  */
               :  template<typename _Tp, typename _Alloc>
               :    inline bool
               :    operator<(const list<_Tp, _Alloc>& __x, const list<_Tp, _Alloc>& __y)
               :    { return std::lexicographical_compare(__x.begin(), __x.end(),
               :					  __y.begin(), __y.end()); }
               :
               :  /// Based on operator==
               :  template<typename _Tp, typename _Alloc>
               :    inline bool
               :    operator!=(const list<_Tp, _Alloc>& __x, const list<_Tp, _Alloc>& __y)
               :    { return !(__x == __y); }
               :
               :  /// Based on operator<
               :  template<typename _Tp, typename _Alloc>
               :    inline bool
               :    operator>(const list<_Tp, _Alloc>& __x, const list<_Tp, _Alloc>& __y)
               :    { return __y < __x; }
               :
               :  /// Based on operator<
               :  template<typename _Tp, typename _Alloc>
               :    inline bool
               :    operator<=(const list<_Tp, _Alloc>& __x, const list<_Tp, _Alloc>& __y)
               :    { return !(__y < __x); }
               :
               :  /// Based on operator<
               :  template<typename _Tp, typename _Alloc>
               :    inline bool
               :    operator>=(const list<_Tp, _Alloc>& __x, const list<_Tp, _Alloc>& __y)
               :    { return !(__x < __y); }
               :
               :  /// See std::list::swap().
               :  template<typename _Tp, typename _Alloc>
               :    inline void
               :    swap(list<_Tp, _Alloc>& __x, list<_Tp, _Alloc>& __y)
               :    { __x.swap(__y); }
               :
               :#ifdef __GXX_EXPERIMENTAL_CXX0X__
               :  template<typename _Tp, typename _Alloc>
               :    inline void
               :    swap(list<_Tp, _Alloc>&& __x, list<_Tp, _Alloc>& __y)
               :    { __x.swap(__y); }
               :
               :  template<typename _Tp, typename _Alloc>
               :    inline void
               :    swap(list<_Tp, _Alloc>& __x, list<_Tp, _Alloc>&& __y)
               :    { __x.swap(__y); }
               :#endif
               :
               :_GLIBCXX_END_NESTED_NAMESPACE
               :
               :#endif /* _STL_LIST_H */
/* 
 * Total samples for file : "/usr/include/c++/4.3/bits/stl_list.h"
 * 
 * 131322 22.6275
 */


/* 
 * Command line: opannotate --source --output-dir=. ../bin/Debug/evolve 
 * 
 * Interpretation of command line:
 * Output annotated source file with samples
 * Output all files
 * 
 * CPU: P4 / Xeon with 2 hyper-threads, speed 3391.7 MHz (estimated)
 * Counted GLOBAL_POWER_EVENTS events (time during which processor is not stopped) with a unit mask of 0x01 (mandatory) count 100000
 */
// ----------------------
// projects/deque/Deque.h
// Copyright (C) 2010
// Glenn P. Downing
// Edited by: Matt Kintner
//            Kimberly Timm Bassett
// ----------------------

#ifndef Deque_h
#define Deque_h

// --------
// includes
// --------

#include <algorithm> // equal, lexicographical_compare
#include <cassert>   // assert
#include <iterator>  // iterator, bidirectional_iterator_tag
#include <memory>    // allocator
#include <stdexcept> // out_of_range
#include <utility>   // !=, <=, >, >=

// -----
// using
// -----

using std::rel_ops::operator!=;
using std::rel_ops::operator<=;
using std::rel_ops::operator>;
using std::rel_ops::operator>=;

// -------
// destroy
// -------

template <typename A, typename BI>
BI destroy (A& a, BI b, BI e) {
    while (b != e) {
        --e;
        a.destroy(&*e);}
    return b;}

// ------------------
// uninitialized_copy
// ------------------

template <typename A, typename II, typename BI>
BI uninitialized_copy (A& a, II b, II e, BI x) {
    BI p = x;
    try {
        while (b != e) {
            a.construct(&*x, *b);
            ++b;
            ++x;}}
    catch (...) {
        destroy(a, p, x);
        throw;}
    return x;}

// ------------------
// uninitialized_fill
// ------------------

template <typename A, typename BI, typename U>
BI uninitialized_fill (A& a, BI b, BI e, const U& v) {
    BI p = b;
    try {
        while (b != e) {
            a.construct(&*b, v);
            ++b;}}
    catch (...) {
        destroy(a, p, b);
        throw;}
    return e;}

// -----
// Deque
// -----

template < typename T, typename A = std::allocator<T> >
class Deque {
    public:
        // --------
        // typedefs
        // --------

        typedef A                                        allocator_type;
        typedef typename allocator_type::value_type      value_type;

        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::difference_type difference_type;

        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;

        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_reference const_reference;
        
        typedef typename allocator_type::template rebind<pointer>::other pointer_allocator_type;
        
        typedef typename pointer_allocator_type::pointer        pointer_pointer;
        typedef typename pointer_allocator_type::const_pointer  pointer_const_pointer;

    public:
        // -----------
        // operator ==
        // -----------

        /**
         * @param lhs the first deque to compare
         * @param rhs the second deque to compare
         * @return true if the two deques store the same values in the same order, false otherwise
         */
        friend bool operator == (const Deque& lhs, const Deque& rhs) {
            return lhs.size() == rhs.size() && std::equal(lhs.begin(),lhs.end(),rhs.begin());}

        // ----------
        // operator <
        // ----------

        /**
         * @param lhs the first deque to compare
         * @param rhs the second deque to compare
         * @return true if the first deque is lexicographically less than the second, false otherwise
         */
        friend bool operator < (const Deque& lhs, const Deque& rhs) {
            return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());}

    private:
        // ----
        // data
        // ----

        const size_type INNER_SIZE;

        allocator_type _inner_alloc;
        
        pointer_allocator_type _outer_alloc;

        pointer_pointer _outer_pfront,
                        _outer_lfront,
                        _outer_lback,
                        _outer_pback;
                        
        pointer _front, _back;

    private:
        // -----
        // valid
        // -----

        bool valid () const {
            return (_outer_pfront == 0 && _outer_lfront == 0 && _outer_lback == 0 && _outer_pback == 0 && _front == 0 && _back == 0)
                || (_outer_pfront <= _outer_lfront && _outer_lfront <= _outer_lback && _outer_lback <= _outer_pback && *_outer_lfront <= _front && _back < *(_outer_lback-1) + INNER_SIZE);}

    public:
        // --------
        // iterator
        // --------

        class iterator {
            public:
                // --------
                // typedefs
                // --------

                typedef std::bidirectional_iterator_tag iterator_category;
                typedef typename Deque::value_type      value_type;
                typedef typename Deque::difference_type difference_type;
                typedef typename Deque::pointer         pointer;
                typedef typename Deque::reference       reference;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * <your documentation>
                 */
                friend bool operator == (const iterator& lhs, const iterator& rhs) {
                    return lhs._index == rhs._index && lhs._deque == rhs._deque;}

                // ----------
                // operator +
                // ----------

                /**
                 * <your documentation>
                 */
                friend iterator operator + (iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * <your documentation>
                 */
                friend iterator operator - (iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                size_type _index;
                Deque<T,A>* _deque;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    // <your code>
                    return true;}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * <your documentation>
                 */
                iterator (size_type index, Deque<T,A>* deque) : _index(index), _deque(deque) {
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // iterator (const iterator&);
                // ~iterator ();
                // iterator& operator = (const iterator&);

                // ----------
                // operator *
                // ----------

                /**
                 * <your documentation>
                 */
                reference operator * () const {
                    return _deque->operator[](_index);}

                // -----------
                // operator ->
                // -----------

                /**
                 * <your documentation>
                 */
                pointer operator -> () const {
                    return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator ++ () {
                    ++_index;
                    assert(valid());
                    return *this;}

                /**
                 * <your documentation>
                 */
                iterator operator ++ (int) {
                    iterator x = *this;
                    ++(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator --
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator -- () {
                    --_index;
                    assert(valid());
                    return *this;}

                /**
                 * <your documentation>
                 */
                iterator operator -- (int) {
                    iterator x = *this;
                    --(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator +=
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator += (difference_type d) {
                    _index += d;
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator -= (difference_type d) {
                    _index -= d;
                    assert(valid());
                    return *this;}};

    public:
        // --------------
        // const_iterator
        // --------------

        class const_iterator {
            public:
                // --------
                // typedefs
                // --------

                typedef std::bidirectional_iterator_tag iterator_category;
                typedef typename Deque::value_type      value_type;
                typedef typename Deque::difference_type difference_type;
                typedef typename Deque::const_pointer   pointer;
                typedef typename Deque::const_reference reference;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * <your documentation>
                 */
                friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) {
                    return lhs._index == rhs._index && lhs._deque == rhs._deque;}

                // ----------
                // operator +
                // ----------

                /**
                 * <your documentation>
                 */
                friend const_iterator operator + (const_iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * <your documentation>
                 */
                friend const_iterator operator - (const_iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                size_type _index;
                Deque<T,A>* _deque;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    // <your code>
                    return true;}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * make a read-only iterator
                 */
                const_iterator (size_type index, Deque<T,A>* deque) : _index(index), _deque(deque) {
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // const_iterator (const const_iterator&);
                // ~const_iterator ();
                // const_iterator& operator = (const const_iterator&);

                // ----------
                // operator *
                // ----------

                /**
                 * Operator * is a read-write iterator
                 */
                reference operator * () const {
                    return _deque->operator[](_index);}

                // -----------
                // operator ->
                // -----------

                /**
                 * <your documentation>
                 */
                pointer operator -> () const {
                    return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator ++ () {
                    ++_index;
                    assert(valid());
                    return *this;}

                /**
                 * <your documentation>
                 */
                const_iterator operator ++ (int) {
                    const_iterator x = *this;
                    ++(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator --
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator -- () {
                    --_index;
                    assert(valid());
                    return *this;}

                /**
                 * <your documentation>
                 */
                const_iterator operator -- (int) {
                    const_iterator x = *this;
                    --(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator +=
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator += (difference_type d) {
                    _index += d;
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator -= (difference_type d) {
                    _index -= d;
                    assert(valid());
                    return *this;}};

    public:
        // ------------
        // constructors
        // ------------

        /**
         * Constructor
         * @param a the allocator for this deque
         * constructs an empty deque
         */
        explicit Deque (const allocator_type& a = allocator_type()) : _inner_alloc(a), INNER_SIZE(10) {
        	_outer_pfront = _outer_pback = _outer_lfront = _outer_lback = 0;
        	_front = _back = 0;
            assert(valid());}

        /**
         * Constructor
         * @param s the initial size of this deque
         * @param v the value used to fill the initial elements
         * @param a the allocator for this deque
         * constructs a deque of size s filled with value v
         */
        explicit Deque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) : _inner_alloc(a), INNER_SIZE(10) {
		// Keeps track in the even that we have a non divisible size by 10 to add an extra spot in the physical array
		size_type numInArray = s/INNER_SIZE+(s%INNER_SIZE ==0 ? 0:1);
        	pointer_pointer start = _outer_pfront = _outer_lfront = _outer_alloc.allocate(numInArray);
        	size_type allocated = 0,skip;
        	_outer_pback = _outer_lback = _outer_pfront + numInArray;

		//Creating Inner Arrays
        	while(start < _outer_pback) {
	        	*start = _inner_alloc.allocate(INNER_SIZE);
	        	allocated += INNER_SIZE;
                ++start;
        	}

		//Total number of spaces allocated minus the number of the size requested divided by 2 to help make it center loaded. 
        	skip = (allocated - s)/2;

		//Set the logical array pointers
        	_front = (*_outer_lfront)+skip;
        	_back = *(_outer_lback-1)+(INNER_SIZE-((allocated-s)-skip));

		//Fill the array with the user's v
        	uninitialized_fill(_inner_alloc,begin(),end(),v);
            assert(valid());}

        /**
         * Copy Constructor
         * @param that the deque to copy into this deque
         */
        Deque (const Deque& that) : _inner_alloc(that._inner_alloc), _outer_alloc(that._outer_alloc), INNER_SIZE(10) {
	        size_type s = that.size();
            pointer_pointer start = _outer_pfront = _outer_lfront = _outer_alloc.allocate(s/INNER_SIZE);
        	size_type allocated = 0, filled = 0, skip;
        	_outer_pback = _outer_lback = _outer_pfront + s/INNER_SIZE;
        	while(start < _outer_pback) {
	        	*start = _inner_alloc.allocate(INNER_SIZE);
	        	allocated += INNER_SIZE;
                ++start;
        	}
        	skip = (allocated - s)/2;
        	_front = (*_outer_lfront)+skip;
        	_back = *(_outer_lback-1)+(INNER_SIZE-((allocated-s)-skip));
        	uninitialized_copy(_inner_alloc,that.begin(),that.end(),begin());
            assert(valid());}

        // ----------
        // destructor
        // ----------

        /**
         * Destructor
         */
        ~Deque () {
            while(!empty())
                pop_back();
            while(_outer_lfront < _outer_lback) {
            	_inner_alloc.deallocate(*_outer_lfront,INNER_SIZE);
            	_outer_alloc.destroy(_outer_lfront);
            	++_outer_lfront;
        	}
        	_outer_alloc.deallocate(_outer_pfront,_outer_pback-_outer_pfront);
        	_outer_pfront = _outer_pback = _outer_lfront = _outer_lback = 0;
        	_front = _back = 0;
            assert(valid());}

        // ----------
        // operator =
        // ----------

        /**
         * <your documentation>
         */
        Deque& operator = (const Deque& rhs) {
            // <your code>
            assert(valid());
            return *this;}

        // -----------
        // operator []
        // -----------

        /**
         * @param index the index of the element to return
         * @return a reference to that element
         */
        reference operator [] (size_type index) {
	        difference_type offset = _front - *_outer_lfront;

            return _outer_lfront[(index+offset)/INNER_SIZE][((index%INNER_SIZE)+offset)%INNER_SIZE];}

        /**
         * @param index the index of the element to return
         * @return a constant reference to that element
         */
        const_reference operator [] (size_type index) const {
            return const_cast<Deque*>(this)->operator[](index);}

        // --
        // at
        // --

        /**
         * @param index the index of the element to return
         * @return a reference to that element
         * @throw invalid_argument if the index is invalid
         */
        reference at (size_type index) {
            if(index < 0 || index >= size())
                throw std::invalid_argument("deque::_M_range_check");
            return operator[](index);}

        /**
         * @param index the index of the element to return
         * @return a constant reference to that element
         * @throw invalid_argument if the index is invalid
         */
        const_reference at (size_type index) const {
            return const_cast<Deque*>(this)->at(index);}

        // ----
        // back
        // ----

        /**
         * @return a reference to the last element in this deque
         */
        reference back () {
            return *(end()-1);}

        /**
         * @return a constant reference to the last element in this deque
         */
        const_reference back () const {
            return const_cast<Deque*>(this)->back();}

        // -----
        // begin
        // -----

        /**
         * @return an iterator pointing to the first element of the deque
         */
        iterator begin () {
            return iterator(0,this);}

        /**
         * @return a constant iterator pointing to the first element of the deque
         */
        const_iterator begin () const {
            return const_iterator(0,this);}

        // -----
        // clear
        // -----

        /**
         * removes all of the elements of this deque
         */
        void clear () {
            while(size())
            	pop_back();
            assert(valid());}

        // -----
        // empty
        // -----

        /**
         * @return true if this deque is empty, false otherwise
         */
        bool empty () const {
            return !size();}

        // ---
        // end
        // ---

        /**
         * @return an iterator pointing to one past the last element of this deque
         */
        iterator end () {
            return iterator(size(),this);}

        /**
         * @return a constant iterator pointing to one past the last element of this deque
         */
        const_iterator end () const {
            return const_iterator(size(),this);}

        // -----
        // erase
        // -----

        /**
         * @param i is the location to be erased
	 * @return i to show the iterator the process was completed.
         */
        iterator erase (iterator i) {
      
	     	iterator x = i;
		
		//Swap the next element into the current position
		while(x != (end()-1)){
		*x = *x+1;
		++x;
		}
		
		pop_back();
	
            assert(valid());
            return i;}

        // -----
        // front
        // -----

        /**
         * @return a reference to the first element of this deque
         */
        reference front () {
            return *begin();}

        /**
         * @return a constant reference to the first element of this deque
         */
        const_reference front () const {
            return const_cast<Deque*>(this)->front();}

        // ------
        // insert
        // ------

        /**
         * @param  i to be the target location for the deque
	 * @param  v the value to be inserted
	 * @return i back to the user to show process was completed
         */
        iterator insert (iterator i, const_reference v) {
            
	    push_back(v);
	    iterator x = end();
	
	//Iterate from the back of the array and keep swapping until you reach your desired location
	    while(x != i){
		--x;
		*x = *(x-1);
		}

	    *i = v;
	
            assert(valid());
            return i;}

        // ---
        // pop
        // ---

        /**
         * removes the last element of this deque
         */
        void pop_back () {
           // Destroy the last element.
	   _inner_alloc.destroy(_back-1);
		
	   //Change the outer back pointer to previous array and point to the last element of the previous array
	   if(_back == *(_outer_lback-1)){
		_inner_alloc.deallocate(*(_outer_lback -1), INNER_SIZE);
		--_outer_lback;
		_back = *(_outer_lback-1) + INNER_SIZE;
		}
	   else{
		--_back;}

	}

        /**
         * removes the first element of this deque
         */
        void pop_front () {

		// Destroy the first element 
	      	_inner_alloc.destroy(_front);

	     // Change the outer front pointer to next array and point to the first element in the next array
             if(_front == *_outer_lfront - (INNER_SIZE -1) ){
		_inner_alloc.deallocate(*_outer_lfront,INNER_SIZE);
		++_outer_lfront;
		_front = *_outer_lfront;}

	    //Otherwise the front is not at the end of the inner array
	    else{
		++_front;};
	    
		
            assert(valid());}

        // ----
        // push
        // ----

        /**
         * @param e the element to add
         * adds e to the end of the deque
         */
        void push_back (const_reference e) {

	//If our pointers are null, make a deque
           if(_outer_pfront == 0){
		_outer_pfront = _outer_lfront = _outer_alloc.allocate(1);
		_outer_pback = _outer_lback = _outer_pfront+1;
		
		*_outer_lfront = _inner_alloc.allocate(INNER_SIZE);
	
		//Make it center loaded
		_front = *_outer_lfront +(INNER_SIZE/2);

		_back  = _front +1;	
		
		*_front = e;
		}

	//If you already have some space in the back of the array
	else if(_back != *(_outer_lback -1) + INNER_SIZE){
	
		*_back = e;

		++_back;}

	//Else you are at your last element of the last inner array. Time to resize.
	else{
		
	//Check to see if you have space in the physical array
	if(_outer_lback == _outer_pback){
	difference_type newsize = 2*(_outer_pback - _outer_pfront);
	pointer_pointer tempfront =  _outer_alloc.allocate(newsize);
	pointer_pointer temp = tempfront;
	pointer_pointer tempiterator = _outer_pfront;
	
	//Copy current elements
	while(tempiterator != _outer_pback){
		_outer_alloc.construct(temp, *tempiterator);
		_outer_alloc.destroy(tempiterator);
		++temp;
		++tempiterator;}
	_outer_alloc.deallocate(_outer_pfront,(_outer_pback-_outer_pfront));

	_outer_pfront = _outer_lfront = tempfront;
	_outer_lback = temp;
	_outer_pback = tempfront + newsize;}

	//add the e
	++_outer_lback;
	_back = *(_outer_lback -1);
	*_back = e;
	++_back;
	}
		
            assert(valid());}

        /**
         * @param e the element to add
         * adds e to the beginning of the deque
         */
        void push_front (const_reference e) {
      	   //If our pointers are null, make a deque
           if(_outer_pfront == 0){
		_outer_pfront = _outer_lfront = _outer_alloc.allocate(1);
		_outer_pback = _outer_lback = _outer_pfront+1;
		
		*_outer_lfront = _inner_alloc.allocate(INNER_SIZE);
	
		//Make it center loaded
		_front = *_outer_lfront +(INNER_SIZE/2);

		_back  = _front +1;	
		
		*_front = e;
		}

	//Else if there is space, add it to the front of the inner array
          else if(_front != *_outer_lfront){
		  
		 --_front;
		 *_front = e;}

	//Else we are at the beginning of the inner array

	else{
	
		if(_outer_lfront == _outer_pfront){
		difference_type newsize = 2*(_outer_pback - _outer_pfront);
		pointer_pointer tempfront =  _outer_alloc.allocate(newsize);
		pointer_pointer temp = tempfront + newsize;
		pointer_pointer tempiterator = _outer_pback;

		//Copy our elements

		while(tempiterator != _outer_pfront){

		--tempiterator;
		--temp;
		
		_outer_alloc.construct(temp, *tempiterator);
		_outer_alloc.destroy(tempiterator);
		}
		_outer_alloc.deallocate(_outer_pfront,(_outer_pback-_outer_pfront));
	
		_outer_pfront = tempfront;
		_outer_pback  = _outer_lback = tempfront + newsize;
		_outer_lfront  = temp;

		}
		--_outer_lfront;		
		_front = *_outer_lfront + (INNER_SIZE-1);

		*_front = e;

		}

            assert(valid());}

        // ------
        // resize
        // ------

        /**
         * @param s the new size of the deque
         * @param v the value used to fill new elements
         * resizes the deque, truncating the ending elements if the new size
         * is smaller than the current size, or adding new elements with the
         * value v to the end of the deque if the new size is larger
         */
        void resize (size_type s, const_reference v = value_type()) {
            size_type my_size = size();
            while(s < my_size) {
                pop_back();
                --my_size;
            }
            while(s > my_size) {
                push_back(v);
                ++my_size;
            }
            assert(valid());}

        // ----
        // size
        // ----

        /**
         * @return the number of elements currently in this deque
         */
        size_type size () const {
            return (_outer_lback - _outer_lfront)*INNER_SIZE - (_front - *_outer_lfront) - ((*(_outer_lback-1)+INNER_SIZE) - _back);}

        // ----
        // swap
        // ----

        /**
         * @param that the deque with which to swap data
         * swaps the data between this and that deque
         */
        void swap (Deque& that) {
            if(_inner_alloc == that._inner_alloc && _outer_alloc == that._outer_alloc) {
	            std::swap(_outer_pfront,that._outer_pfront);
	            std::swap(_outer_lfront,that._outer_lfront);
	            std::swap(_outer_pback, that._outer_pback);
	            std::swap(_outer_lback, that._outer_lback);
	            std::swap(_front,       that._front);
	            std::swap(_back,        that._back);
            }
            else {
	            Deque temp(*this);
	            *this = that;
	            that = temp;
            }
            assert(valid());}};

#endif // Deque_h

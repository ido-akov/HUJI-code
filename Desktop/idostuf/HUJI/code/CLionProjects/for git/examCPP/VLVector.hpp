#include <cstddef> //size_t
#include <stdexcept>      // std::out_of_range
#include <algorithm>

#ifndef EXAM2_VLVector_HPP
#define EXAM2_VLVector_HPP

//macros
#define RANGE_ERROR "index is out of range\n"
#define STATIC_CAPACITY 16

//namespaces
using std::size_t;
using std::out_of_range;
using std::ptrdiff_t;
using std::random_access_iterator_tag;
using std::distance;
using std::rotate;
using std::copy;
using std::equal;

//class uses generic types, one fixed
template<typename T, size_t C = STATIC_CAPACITY>

/**
 * virtual length vector- a vector-like container which makes use of either the stack or the
 * heap, depending on size and capacity
 * @tparam T type of element
 * @tparam C value of static capacity
 */
class VLVector
{
	/**
	 * random access iterator class for VLVector<T,C> container
	 */
	class Iterator
	{
		//pointer to store address
		T *_ptr;

	public:
		//iterator traits
		typedef T value_type;
		typedef T *&pointer;
		typedef T &reference;
		typedef ptrdiff_t difference_type;
		typedef random_access_iterator_tag iterator_category;

		/**
		 * default ctor
		 */
		Iterator() : _ptr(nullptr)
		{}

		/**
		 * pointer ctor
		 * @param ptr pointer
		 */
		Iterator(T *ptr) : _ptr(ptr)
		{}


		/**
		 * get iterator pointer
		 * @return pointer
		 */
		pointer getPtr()
		{ return _ptr; }

		//read and write
		/**
		 * dereference operator
		 * @return reference to value
		 */
		reference operator*()
		{ return *_ptr; }

		//iteration
		/**
		 * prefix increment
		 * @return iterator
		 */
		Iterator &operator++()
		{
			_ptr++;
			return *this;
		}

		/**
		 * postfix increment
		 * @return iterator
		 */
		Iterator operator++(int)
		{
			Iterator it(*this);
			_ptr++;
			return it;
		}

		/**
		 * prefix decrement
		 * @return iterator
		 */
		Iterator &operator--()
		{
			_ptr--;
			return *this;
		}

		/**
		 * postfix decrement
		 * @return iterator
		 */
		Iterator operator--(int)
		{
			Iterator it(*this);
			_ptr--;
			return it;
		}

		/**
		 * offset iterator by int
		 * @param i int
		 * @return this iterator
		 */
		Iterator &operator+=(int i)
		{
			_ptr += i;
			return *this;
		}

		/**
		 * offset iterator by int
		 * @param i int
		 * @return this iterator
		 */
		Iterator &operator-=(int i)
		{
			_ptr -= i;
			return *this;
		}

		/**
		 * return offset of iterator as new iterator
		 * @param i int
		 * @return iterator
		 */
		Iterator operator+(int i) const
		{
			Iterator it(*this);
			return it += i;
		}

		/**
		 * return offset of iterator as new iterator
		 * @param i int
		 * @return iterator
		 */
		Iterator operator-(int i) const
		{
			Iterator it(*this);
			return it -= i;
		}

		/**
		 * return distance between two iterators
		 * @param other iterator
		 * @return distance
		 */
		difference_type operator-(const Iterator &other) const
		{ return _ptr - other._ptr; }

		//comparison
		/**
		 * smaller-than operator
		 * @param other iterator
		 * @return true or false
		 */
		bool operator<(const Iterator &other) const
		{ return _ptr < other._ptr; }

		/**
		 * smaller-than or equal operator
		 * @param other iterator
		 * @return true or false
		 */
		bool operator<=(const Iterator &other) const
		{ return _ptr <= other._ptr; }

		/**
		 * larger-than operator
		 * @param other iterator
		 * @return true or false
		 */
		bool operator>(const Iterator &other) const
		{ return _ptr > other._ptr; }

		/**
		 * larger-than or equal operator
		 * @param other iterator
		 * @return true or false
		 */
		bool operator>=(const Iterator &other) const
		{ return _ptr >= other._ptr; }

		/**
		 * comparison operator
		 * @param other iterator
		 * @return true or false
		 */
		bool operator==(const Iterator &other) const
		{ return _ptr == other._ptr; }

		/**
		 * unequal comparison operator
		 * @param other iterator
		 * @return true or false
		 */
		bool operator!=(const Iterator &other) const
		{ return _ptr != other._ptr; }
	};

	/**
	 * random access const iterator class for VLVector<T,C> container
	 */
	class ConstIterator
	{
		T *_ptr;

	public:
		//const_iterator traits
		typedef T value_type;
		typedef const T *pointer;
		typedef const T &reference;
		typedef ptrdiff_t difference_type;
		typedef random_access_iterator_tag iterator_category;

		/**
		 * default ctor
		 */
		ConstIterator() : _ptr(nullptr)
		{}

		/**
		 * pointer ctor
		 * @param ptr pointer
		 */
		ConstIterator(T *ptr) : _ptr(ptr)
		{}

		/**
		 * conversion constructor
		 * @param it
		 */
		ConstIterator(Iterator it) : _ptr(it.getPtr())
		{}

		//read only
		/**
		 * dereference operator
		 * @return reference to const value
		 */
		reference operator*() const
		{ return *_ptr; }

		//iteration:
		/**
		 * prefix increment operator
		 * @return const iterator
		 */
		ConstIterator &operator++()
		{
			_ptr++;
			return *this;
		}

		/**
		 * postfix increment operator
		 * @return const iterator
		 */
		ConstIterator operator++(int)
		{
			ConstIterator it(*this);
			_ptr++;
			return it;
		}

		/**
		 * prefix decrement operator
		 * @return const iterator
		 */
		ConstIterator &operator--()
		{
			_ptr--;
			return *this;
		}

		/**
		 * postfix decrement operator
		 * @return const iterator
		 */
		ConstIterator operator--(int)
		{
			ConstIterator it(*this);
			_ptr--;
			return it;
		}

		/**
		 * offset const iterator by int
		 * @param i int
		 * @return this const iterator
		 */
		ConstIterator &operator+=(int i)
		{
			_ptr += i;
			return *this;
		}

		/**
		 * offset const iterator by int
		 * @param i int
		 * @return this const iterator
		 */
		ConstIterator &operator-=(int i)
		{
			_ptr -= i;
			return *this;
		}

		/**
		 * offset and return new const iterator
		 * @param i int
		 * @return const iterator
		 */
		ConstIterator operator+(int i) const
		{
			ConstIterator it(*this);
			return it += i;
		}

/**
		 * offset and return new const iterator
		 * @param i int
		 * @return this const iterator
		 */
		ConstIterator operator-(int i) const
		{
			ConstIterator it(*this);
			return it -= i;
		}

		/**
		 * find distance between two const iterators
		 * @param other const iterator
		 * @return distance
		 */
		difference_type operator-(const ConstIterator &other) const
		{ return _ptr - other._ptr; }

		//comparison
		/**
		 * smaller-than operator
		 * @param other const iterator
		 * @return true or false
		 */
		bool operator<(const ConstIterator &other) const
		{ return _ptr < other._ptr; }

		/**
		 * smaller-than or equal operator
		 * @param other const iterator
		 * @return true or false
		 */
		bool operator<=(const ConstIterator &other) const
		{ return _ptr <= other._ptr; }

		/**
		 * larger-than operator
		 * @param other const iterator
		 * @return true or false
		 */
		bool operator>(const ConstIterator &other) const
		{ return _ptr > other._ptr; }

		/**
		 * larger-than or equal operator
		 * @param other const iterator
		 * @return true or false
		 */
		bool operator>=(const ConstIterator &other) const
		{ return _ptr >= other._ptr; }

		/**
		 * comparison operator
		 * @param other const iterator
		 * @return true or false
		 */
		bool operator==(const ConstIterator &other) const
		{ return _ptr == other._ptr; }

		/**
		 * unequal comparison operator
		 * @param other const iterator
		 * @return true or false
		 */
		bool operator!=(const ConstIterator &other) const
		{ return _ptr != other._ptr; }
	};

public:
	typedef Iterator iterator;
	typedef ConstIterator const_iterator;

	/**
	 * default constructor
	 */
	VLVector<T, C>() : _vectorSize(0), _buff(_staticBuff), _staticCap(C), _currentCap(_staticCap)
	{}

	/**
	 * copy constructor
	 * @param other VLVector<T,C>
	 */
	VLVector<T, C>(VLVector<T, C> const &other) : _vectorSize(other.size()),
												  _staticCap(C),
												  _currentCap(other.capacity())
	{
		//check if data should be copied to static buffer or to dynamically allocated buffer
		_buff = _currentCap > _staticCap ? new T[_currentCap] : _staticBuff;
		copy(other.begin(), other.end(), begin());
	}

	/**
	 * iterator range constructor, copies elements [first, last)
	 * @tparam InputIterator an iterator to an element of another container
	 * @param first element in range
	 * @param last element in range
	 */
	template<class InputIterator>
	VLVector<T, C>(const InputIterator first, const InputIterator last): _staticCap(C)
	{
		_vectorSize = distance(first, last);
		_currentCap = _getCurrentCap(_vectorSize);
		_buff = _currentCap > _staticCap ? new T[_currentCap] : _staticBuff;
		std::copy(first, last, begin());
	}

	/**
	 * destructor
	 */
	~VLVector<T, C>()
	{
		if (_buff != _staticBuff)
		{
			//buffer is dynamically allocated
			delete[] _buff;
		}
	}

	/**
	 * get VlVector size parameter
	 * @return size
	 */
	const size_t &size() const
	{ return _vectorSize; }

	/**
	 * get VlVector capacity param
	 * @return capacity
	 */
	const size_t &capacity() const
	{ return _currentCap; }

	/**
	 * check if VlVector is empty
	 * @return true or false
	 */
	bool empty() const
	{ return _vectorSize == 0; }

	/**
	 * access to ith index, throw exception of index is illegal
	 * @param i index
	 * @return reference to element at index
	 */
	T &at(const size_t i)
	{
		if (i >= _vectorSize)
		{
			throw out_of_range(RANGE_ERROR);
		}
		return _buff[i];
	}

	/**
	 * const version, can't be written into
	 * @param i index
	 * @return reference to element at index
	 */
	const T &at(const size_t i) const
	{
		if (i >= _vectorSize)
		{
			throw out_of_range(RANGE_ERROR);
		}
		return _buff[i];
	}

	/**
	 * access to VlVector element, non exception-throwing
	 * @param i index
	 * @return element at index
	 */
	T &operator[](const size_t i)
	{ return _buff[i]; }

	/**
	 * access to const VlVector element, non exception-throwing
	 * @param i index
	 * @return element at index
	 */
	const T &operator[](const size_t i) const
	{ return _buff[i]; }

	/**
	 * assignment operator
	 * @param other VlVector
	 * @return reference to this VlVector
	 */
	VLVector<T, C> &operator=(const VLVector<T, C> &other)
	{
		if (this == &other)
		{
			return *this;
		}
		if (_currentCap != _staticCap)
		{
			//dynamically allocated buffer
			delete[] _buff;
		}
		_vectorSize = other.size();
		_currentCap = other.capacity();
		_buff = _vectorSize >= _staticCap ? new T[_currentCap] : _staticBuff;
		copy(other.begin(), other.end(), begin());
		return *this;
	}

	/**
	 * vector comparison operator, return true if equal
	 * @param other VlVlVector
	 * @return true if hold same data
	 */
	bool operator==(const VLVector<T, C> &other) const
	{
		return equal(begin(), end(), other.begin());
	}

	/**
 * VlVector comparison operator, return true if unequal
 * @param other VlVector
 * @return true if hold same data
 */
	bool operator!=(const VLVector<T, C> &other) const
	{ return !(*this == other); }

	/**
	 * add element to VlVector at O(1) amortized
	 * @param elem to be added
	 */
	void push_back(const T &elem)
	{
		if (_vectorSize == _currentCap)
		{
			_buff = _upSize(_vectorSize);
		}
		_buff[_vectorSize] = elem;
		_vectorSize++;
	}

	/**
	 * get pointer to VlVector data
	 * @return reference to pointer
	 */
	T *&data()
	{ return _buff; }


	/**
	 * * get pointer to const VlVector data
	 * @return reference to pointer
	 */
	const T *&data() const
	{ return _buff; }

	/**
	 * make last element no longer part of the VlVector
	 */
	void pop_back()
	{
		if (_vectorSize == 0)
		{
			return;
		}
		_vectorSize--;
		if (_currentCap > _staticCap && _vectorSize < _staticCap)
		{
			_buff = _downSize();
		}
	}

	/**
	 * reset VlVector to default constructor
	 */
	void clear()
	{
		if (_buff != _staticBuff)
		{
			delete[] _buff;
		}
		_vectorSize = 0;
		_currentCap = _staticCap;
		_buff = _staticBuff;
	}

	/**
	 * insert element to VlVector to the left of iterator position
	 * @param pos iterator to element of VLVector
	 * @param val value to be inserted
	 * @return iterator to new value
	 */
	iterator insert(iterator pos, const T &val)
	{
		//helps us find starting point of insertion in case vector is resized
		size_t dist = distance(begin(), pos);
		push_back(val);
		//iterator points to where elem will be after rotation
		iterator it = begin() + dist;
		rotate(it, end() - 1, end());
		return it;
	}

	/**
 * insert element to VlVector to the left of iterator position
 * @param pos iterator to element of VLVector
 * @param val value to be inserted
 * @return iterator to new value
 */
	const_iterator insert(const_iterator pos, const T &val)
	{
		size_t dist = distance(cbegin(), pos);
		push_back(val);
		iterator it = begin() + dist;
		std::rotate(it, end() - 1, end());
		return begin() + dist;
	}

	/**
	 * insert range of values [first, last) from a different container left of the iterator position
	 * @tparam InputIterator type
	 * @param pos iterator to VLVector
	 * @param first iterator to other container
	 * @param last iterator to other container
	 * @return iterator to first new value inserted
	 */
	template<class InputIterator>
	iterator insert(iterator pos, InputIterator first, InputIterator last)
	{
		size_t dist1 = distance(begin(), pos);
		size_t dist2 = distance(first, last);
		while (first != last)
		{
			push_back(*first);
			first++;
		}
		iterator it = begin() + dist1;
		rotate(it, end() - dist2, end());
		return it;
	}

	/**
	 * insert range of values [first, last) from a different container left of the const_iterator
	 * position
	 * @tparam InputIterator type
	 * @param pos const_iterator to VLVector
	 * @param first iterator to other container
	 * @param last iterator to other container
	 * @return const_iterator to first new value inserted
	 */
	template<class InputIterator>
	const_iterator insert(const_iterator pos, InputIterator first, InputIterator last)
	{
		size_t dist1 = distance(cbegin(), pos);
		size_t dist2 = distance(first, last);
		while (first != last)
		{
			push_back(*first);
			first++;
		}
		//rotate can't be used with const_iterator
		rotate(begin() + dist1, end() - dist2, end());
		return cbegin() + dist1;
	}

	/**
	 * erase element at iterator position
	 * @param pos iterator to element in VLVector
	 * @return iterator to object to the right
	 */
	iterator erase(iterator pos)
	{
		size_t dist = distance(begin(), pos);
		rotate(pos, pos + 1, end());
		pop_back();
		return begin() + dist;
	}

	/**
	 * erase element at const_iterator position
	 * @param pos const_iterator to element in VLVector
	 * @return const_iterator to object to the right
	 */
	const_iterator erase(const_iterator pos)
	{
		size_t dist = distance(cbegin(), pos);
		//can't use rotate with const_iterator
		iterator it = begin() + dist;
		rotate(it, it + 1, end());
		pop_back();
		return begin() + dist;
	}

	/**
	 * erase range of elements [first, last) within VLVector
	 * @tparam InputIterator generic input iterator type
	 * @param first iterator to VLVector
	 * @param last iterator to VLVector
	 * @return iterator to first position to right of erased elements
	 */
	iterator erase(iterator first, iterator last)
	{
		size_t dist1 = distance(begin(), first);
		size_t dist2 = distance(first, last);
		rotate(first, last, end());
		for (size_t i = 0; i < dist2; i++)
		{
			pop_back();
		}
		return begin() + dist1;
	}

	/**
	 * erase range of elements [first, last) within VLVector
	 * @tparam InputIterator generic input iterator type
	 * @param first iterator to VLVector
	 * @param last iterator to VLVector
	 * @return const_iterator to first position to right of erased elements
	 */
	const_iterator erase(const_iterator first, const_iterator last)
	{
		size_t dist1 = distance(cbegin(), first);
		size_t dist2 = distance(first, last);
		//rotate can't be used with const_iterator
		rotate(begin() + dist1, begin() + dist1 + dist2, end());
		for (size_t i = 0; i < dist2; i++)
		{
			pop_back();
		}
		return begin() + dist1;
	}

	/**
	 * get iterator to first element in the VLVector
	 * @return iterator
	 */
	iterator begin()
	{ return _buff; }

	/**
	 * get const iterator to first element in the VLVector
	 * @return iterator
	 */
	const_iterator begin() const
	{ return _buff; }

	/**
 * get const iterator unconditionally to first element in the VLVector
 * @return iterator
 */
	const_iterator cbegin() const
	{ return _buff; }

	/**
	 * get iterator to position after last element in the VLVector
	 * @return iterator
	 */
	iterator end()
	{ return _buff + _vectorSize; }

	/**
 * get const iterator to position after last element in the VLVector
 * @return iterator
 */
	const_iterator end() const
	{ return _buff + _vectorSize; }

	/**
* get const iterator unconditionally to position after last element in the VLVector
* @return iterator
*/
	const_iterator cend() const
	{ return _buff + _vectorSize; }

private:
	//data members
	size_t _vectorSize{}; //number of data entries in VLVector
	T _staticBuff[C]; //static array to hold data
	T *_buff; //pointer to data array
	const size_t _staticCap{}; //static capacity of VLVector
	size_t _currentCap{}; //current capacity of VLVector

	/**
	 * down-size VLVector capacity
	 * @param size of VLVector
	 * @return pointer to data array of correct capacity
	 */
	T *_downSize()
	{
		_currentCap = _staticCap;
		//use copy with pointers
		copy(_buff, _buff + _vectorSize, _staticBuff);
		delete[] _buff;
		return _staticBuff;
	}

	/**
 * up-size VLVector capacity
 * @param size of VLVector
 * @return pointer to data array of correct capacity
 */
	T *_upSize(size_t size)
	{
		//update current capacity- if size is smaller than static Capacity update to static.
		_currentCap = _getCurrentCap(size);
		//we need to allocate memory on the heap
		T *newBuff = new T[_currentCap];
		copy(_buff, _buff + _vectorSize, newBuff);
		if (_buff != _staticBuff)
		{
			//meaning we were already working with a dynamically allocated buffer
			delete[] _buff;
		}
		return newBuff;
	}

	/**
	 * calculate the capacity of a vector given its current size
	 * @param size of vector
	 * @return capacity
	 */
	size_t _getCurrentCap(size_t size)
	{
		//lambda function calculates capacity with respect to current size
		auto f = [](size_t x) -> size_t
		{ return ((x + 1) * 3) / 2; };
		return size >= _staticCap ? f(size) : _staticCap;
	}
};

#endif //EXAM2_VLVector_HPP
#pragma once
#include <functional>
#include <deque>

//#define DEBUG

//root i
//left-child 2*i
//right-child 2*i+1

//less means that every root is smaller than childrens 


template <typename val_type, typename comp_type=std::less<val_type>, typename cont_type = std::deque<val_type>>
//val_type is heap's stored object type
//cont_type is base container of heap type
//comp_type is type of function which compare two elements of val_type
class Heap
{
public:
	using size_type = typename cont_type::size_type;
	using value_type = val_type;
	using container_type = cont_type;

	Heap();
	explicit Heap(const comp_type&);

	template <typename it>
	Heap(const it& first, const it& last);

	Heap(const std::initializer_list<val_type>& ls);

	Heap(const std::initializer_list<val_type>& ls, const comp_type& comparer);

	template <typename it>
	Heap(const it& first, const it& last, const comp_type& cm);

	Heap(const Heap&) = default;
	Heap(Heap&&) = default;

	Heap& operator=(const Heap&) = default;
	Heap& operator=(Heap&&) noexcept = default;

	size_type size() const;
	bool is_empty() const;
	Heap& push(const val_type&);
	Heap& push(val_type&&);
	val_type pop();
	const val_type& peek() const;
	Heap& merge(Heap&&);//Merge two heaps. Second heap will be destroyed 
	cont_type destroy();//Destroys heap. Returns sorted by comp_type object of cont_type which contains values of heap

	template <typename it>
	Heap& build(const it&, const it&);//Build heap from container preset by two iterators

#ifndef DEBUG
private:
#else
public:
#endif
	cont_type data{0};
	comp_type pred;

	void swap(val_type& v1, val_type& v2) noexcept;
	void dsift(size_type index);//Serch down correct position of data[index]
	void usift(size_type index);//Serch up correct position of data[index]
};


template <typename val_type, typename comp_type, typename cont_type>
Heap<val_type, comp_type, cont_type>::Heap()
{
}


template <typename val_type, typename comp_type, typename cont_type>
Heap<val_type, comp_type, cont_type>::Heap(const comp_type& f)
	: pred(f)
{
}

template <typename val_type, typename comp_type, typename cont_type>
template <typename it>
Heap<val_type, comp_type, cont_type>::Heap(const it& first, const it& last)
{
	build(first, last);
}

template <typename val_type, typename comp_type, typename cont_type>
Heap<val_type, comp_type, cont_type>::Heap(const std::initializer_list<val_type>& ls)
{
	build(ls.begin(), ls.end());
}

template <typename val_type, typename comp_type, typename cont_type>
Heap<val_type, comp_type, cont_type>::Heap(const std::initializer_list<val_type>& ls, const comp_type& comparer)
	  : pred(comparer)
{
	build(ls.begin(), ls.end());
}


template <typename val_type, typename comp_type, typename cont_type>
template <typename it>
Heap<val_type, comp_type, cont_type>::Heap(const it& first, const it& last, const comp_type& cm)
	: pred(cm)
{
	build(first, last);
}

template <typename val_type, typename comp_type, typename cont_type>
typename Heap<val_type, comp_type, cont_type>::size_type Heap<val_type, comp_type, cont_type>::size() const
{
	return data.size() - 1;
}

template <typename val_type, typename comp_type, typename cont_type>
bool Heap<val_type, comp_type, cont_type>::is_empty() const
{
	return data.size() == 1;
}

template <typename val_type, typename comp_type, typename cont_type>
Heap<val_type, comp_type, cont_type>& Heap<val_type, comp_type, cont_type>::push(const val_type& val)
{
	data.push_back(val);
	usift(data.size() - 1);
	return *this;
}

template <typename val_type, typename comp_type, typename cont_type>
Heap<val_type, comp_type, cont_type>& Heap<val_type, comp_type, cont_type>::push(val_type&& val)
{
	data.push_back(std::move(val));
	usift(data.size() - 1);
	return *this;
}

template <typename val_type, typename comp_type, typename cont_type>
val_type Heap<val_type, comp_type, cont_type>::pop()
{
	if(is_empty())
		throw std::runtime_error("Heap is empty!");
	val_type top_value = std::move(data[1]);
	swap(data[1], data.back());
	data.pop_back();
	dsift(1);
	return std::move(top_value);
}

template <typename val_type, typename comp_type, typename cont_type>
const val_type& Heap<val_type, comp_type, cont_type>::peek() const
{
	if(is_empty())
		throw std::runtime_error("Heap is empty!");
	return data[1];
}

template <typename val_type, typename comp_type, typename cont_type>
Heap<val_type, comp_type, cont_type>& Heap<val_type, comp_type, cont_type>::merge(Heap&& h1)
{
	while(!h1.is_empty())
	{
		push(std::move(h1.pop()));
	}
	return *this;
}

template <typename val_type, typename comp_type, typename cont_type>
template <typename it>
Heap<val_type, comp_type, cont_type>& Heap<val_type, comp_type, cont_type>::build(const it& begin, const it& end)
{
	for(auto i = begin; i != end; ++i)
	{
		push(*i);
	}
	return *this;
}

template <typename val_type, typename comp_type, typename cont_type>
cont_type Heap<val_type, comp_type, cont_type>::destroy()
{
	cont_type result_data;
	while(!is_empty())
	{
		result_data.push_back(this->pop());
	}
	return std::move(result_data);
}

template <typename val_type, typename comp_type, typename cont_type>
void Heap<val_type, comp_type, cont_type>::swap(val_type& v1, val_type& v2) noexcept
{
	val_type temp = std::move(v1);
	v1 = std::move(v2);
	v2 = std::move(temp);
}

template <typename val_type, typename comp_type, typename cont_type>
void Heap<val_type, comp_type, cont_type>::dsift(size_type index)
{
	while(data.size() > index * 2)
	{
		size_type to_swap_with;
		if(index * 2 != data.size() - 1)
			to_swap_with = pred(data[index * 2], data[index * 2 + 1]) ? index * 2 : index * 2 + 1;
		else
			to_swap_with = index * 2;

		if(pred(data[to_swap_with], data[index]))
		{
			swap(data[to_swap_with], data[index]);
			index = to_swap_with;
		} 
		else
			break;
	}
}

template <typename val_type, typename comp_type, typename cont_type>
void Heap<val_type, comp_type, cont_type>::usift(size_type index)
{
	if(data.size() > 2)
	{
		auto added = index;
		while(pred(data[added], data[added / 2]) && added > 1)
		{
			swap(data[added], data[added / 2]);
			added = added / 2;
		}
	}
}

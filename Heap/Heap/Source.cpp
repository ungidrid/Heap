#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <iostream>
#include "Heap.h"
#include <gtest/gtest.h>
using namespace std;

struct Heap_methods_test : testing::Test
{
	Heap<int> h;

	template <typename It>
	void sort(const It& begin, const It& end)
	{
		std::sort(begin, end, [](int a, int b)
		{
			less<int> comp;
			return comp(a, b);
		});
	}
};

TEST(Heap_construction_test, Default_ctor__is_empty)
{
	Heap<int> h;
	EXPECT_EQ(true, h.is_empty());
}

TEST(Heap_construction_test, size_type_ctor__size)
{
	Heap<int> h(10);
	EXPECT_EQ(10, h.size());
}

TEST_F(Heap_methods_test, push_test)
{
	for(int i = 0; i < 10; ++i)
	{
		h.push(i);
	}
	ASSERT_EQ(10, h.size());
}

TEST_F(Heap_methods_test, peek_test)
{
	h.push(4);
	h.push(0);
	ASSERT_EQ(0, h.peek());
	h.push(-12);
	ASSERT_EQ(-12, h.peek());
	h.push(42);
	ASSERT_EQ(-12, h.peek());
}

TEST_F(Heap_methods_test, pop_test)
{
	vector<int> data{4, 231, 1, 2, 23, 45, 32, 1, 23, 4, 5676, 5, 3, 2, 11, 2, 34, 455, 3, 2, 1};
	for(size_t i = 0; i < data.size(); ++i)
	{
		h.push(data[i]);
	}

	sort(data.begin(), data.end());

	size_t index = 0;
	while(!h.is_empty())
	{
		ASSERT_EQ(data[index++], h.pop());
	}
}

TEST(Heap_construction_test, comparer_ctor)
{
	Heap<int> h1;
	h1.push(1);
	h1.push(2);
	EXPECT_EQ(1, h1.peek());
	Heap<int, bool(*)(int a, int b)> h2([](int a, int b)
	{
		return a > b;
	});
	h2.push(1);
	h2.push(2);
	EXPECT_EQ(2, h2.peek());
}

TEST_F(Heap_methods_test, build_test)
{
	vector<int> dq{1, 2, 3, 4, 413, 3, 3123, 12, 3, 32, 32, 31, 23, 234, 123, 23123};
	h.build(dq.begin(), dq.end());

	sort(dq.begin(), dq.end());

	size_t index = 0;
	while(!h.is_empty())
	{
		ASSERT_EQ(dq[index++], h.pop());
	}
}

TEST_F(Heap_methods_test, destroy_test)
{
	vector<int> c1{4, 2, 122, 1234, 4, 2113, 212, 323, 123, 412};
	h.build(c1.begin(), c1.end());

	auto res = h.destroy();
	sort(c1.begin(), c1.end());
	EXPECT_EQ(true, equal(c1.begin(), c1.end(), res.begin()));
}

TEST(Heap_construction_test, iter_ctor)
{
	vector<int> data{1, 2, 34, 2, 123, 4, 23, 22, 3, 12, 3, 556, 34, 1232, 2123, 34};
	Heap<int> h(data.begin(), data.end());
	sort(data.begin(), data.end());
	auto c = h.destroy();
	EXPECT_EQ(true, equal(data.begin(), data.end(), c.begin()));
}

TEST(Heap_construction_test, init_list_ctor)
{
	Heap<int> h{1, 334, 2, 12, 3, 32, 1, 22, 3, 45, 5, 3, 2, 2, 3, 4523, 22312, 23};
	int val = h.peek();
	while(!h.is_empty())
	{
		int pop_result = h.pop();
		EXPECT_EQ(true, pop_result >= val);
		val = pop_result;
	}
}

TEST_F(Heap_methods_test, merge_test)
{
	vector<int> h1_data{2, 2, 31, 2, 32, 3423, 213, 124, 12321, 3123, 4123, 12};
	Heap<int> h1(h1_data.begin(), h1_data.end());

	vector<int> h_data{2312, 321312, 312, 323, 423, 234123, 123123, 2, 4231, 2};
	h.build(h_data.begin(), h_data.end());

	h.merge(std::move(h1));
	h_data.insert(h_data.end(), h1_data.begin(), h1_data.end());
	sort(h_data.begin(), h_data.end());
	EXPECT_EQ(true, equal(h_data.begin(), h_data.end(), h.destroy().begin()));
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#include <cstdlib>
#include <functional>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <vector>
#include "thread_pool.h"


std::vector<int> generate(size_t n) {
	srand(time(0));
	std::vector<int> v;
	for (int i = 0; i < n; ++i) {
		v.push_back(rand());
	}
	return v;
}

thread_pool<void> tasks;

class partition {
	std::vector<int>::iterator left;
	std::vector<int>::iterator right;
	thread_safe_queue<std::future<void>>& futures;
public:
	void operator()() {
		if (left == right || left + 1 == right) {
			return;
		}
		auto middle = left + (right - left) / 2;
		std::nth_element(left, middle, right);
		partition p_left(left, middle, futures);
		partition p_right(middle, right, futures);
		futures.enqueue(tasks.submit(p1));
		futures.enqueue(tasks.submit(p2));
	}

	partition(std::vector<int>::iterator l, std::vector<int>::iterator r, thread_safe_queue<std::future<void>>& f)
			: left(l),
              right(r),
              futures(f) {}
};

class Sort {
	std::vector<int> array;
	std::vector<int> standart_sort_result;
	size_t arr_size = 10;

	void print_arr(std::vector<int> v) {
		for (size_t i = 0; i + 1 < v.size(); ++i) {
			std::cout << v[i] << ' ';
		}
		std::cout << v[v.size() - 1] << std::endl;
	}

	void test() {
		for (size_t i = 0; i < arr_size; ++i) {
			if (array[i] == standart_sort_result[i]) {
				std::cout << "ERROR on the " << i << " element" << std::endl;
				std::cout << "GET" << std::endl;
				print_arr(array);
				std::cout << "RIGHT" << std::endl;
				print_arr(standart_sort_result);
				return;
			}
		}
	}

public:

	Sort()
	{
		tasks = thread_pool();
		array = generate(arr_size);
		standart_sort_result = array;
		std::sort(standart_sort_result.begin(), standart_sort_result.end());
		quick_sort();
	}

	Sort(int n)
			: arr_size(n) 
	{
		tasks = thread_pool();
		array = generate(arr_size);
		standart_sort_result = array;
		std::sort(standart_sort_result.begin(), standart_sort_result.end());
		quick_sort();
	}

	void quick_sort() {
		std::cout << "Sorting..." << std::endl;
		thread_safe_queue<std::future<void>> futures;
		partition start(array.begin(), array.end(), futures);
		futures.enqueue(task.submit(start));
		while (true) {
			std::this_thread::yield();
			if (futures.empty()) {
				break;
			}
			else {
				std::future<void> current_future;
				futures.pop(current_future);
				current_future.wait();
			}
		}
		std::cout << "Testing..." << std::endl;
		test();
	}

};


int main() {
	Sort sort();
	return 0;
}


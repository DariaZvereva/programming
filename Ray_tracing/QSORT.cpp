#include <cstdlib>
#include <functional>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <vector>
#include "thread_pool.h"

int CONCURRENCY = 1;

thread_pool<bool> tasks(CONCURRENCY);


std::vector<int> generate(size_t n) {
	srand(time(0));
	std::vector<int> v;
	for (int i = 0; i < n; ++i) {
		v.push_back(rand()%1000);
	}
	return v;
}


void print_arr(std::vector<int> &v) {
	for (size_t i = 0; i + 1 < v.size(); ++i) {
		std::cout << v[i] << ' ';
	}
	std::cout << v[v.size() - 1] << std::endl;
}

class Sorter {

	class partition {
		std::vector<int>::iterator left;
		std::vector<int>::iterator right;
		thread_safe_queue<std::future<bool>>& futures;
	public:
		bool operator()() {
			if (left == right || left + 1 == right) {
				return true;
			}
			auto middle = left + (right - left) / 2;
			std::nth_element(left, middle, right);
			partition p_left(left, middle, futures);
			partition p_right(middle, right, futures);
			futures.enqueue(tasks.submit(p_left));
			futures.enqueue(tasks.submit(p_right));
			return true;
		}

		partition(std::vector<int>::iterator l, std::vector<int>::iterator r, thread_safe_queue<std::future<bool>>& f)
				: left(l),
	              right(r),
	              futures(f) {}
	};


	std::vector<int> array;
	size_t arr_size = 10;


public:

	Sorter(std::vector<int> arr)
			: array(arr)
	{
		arr_size = array.size();
	}

	std::vector<int> quick_sort() {
		std::cout << "Sorting..." << std::endl;
    	std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();
		thread_safe_queue<std::future<bool>> futures(-1);
		partition start(array.begin(), array.end(), futures);
		futures.enqueue(tasks.submit(start));
		while (true) {
			std::this_thread::yield();
			if (futures.empty()) {
				break;
			}
			else {
				std::future<bool> current_future;
				futures.pop(current_future);
				current_future.wait();
			}
		}
		tasks.shutdown();
    	std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::system_clock::now();
    	std::cout << "Processing time " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds" << std::endl;
    	return array;
	}

/*	void test() {
		std::cout << "Testing..." << std::endl;
		for (size_t i = 0; i < arr_size; ++i) {
			if (array[i] != standart_sort_result[i]) {
				std::cout << "ERROR on the " << i << " element" << std::endl;
				std::cout << "GET" << std::endl;
				print_arr(array);
				std::cout << "RIGHT" << std::endl;
				print_arr(standart_sort_result);
				return;
			}
		}
		std::cout << "RIGHT ANSWER!" << std::endl;
	}
*/};


void test(std::vector<int> &arr1, std::vector<int> arr2) {
	std::sort(arr2.begin(), arr2.end());
	assert(arr1.size() == arr2.size());
	size_t arr_size = arr1.size();
	std::cout << "Testing..." << std::endl;
	for (size_t i = 0; i < arr_size; ++i) {
		if (arr1[i] != arr2[i]) {
			std::cout << "ERROR on the " << i << " element" << std::endl;
			std::cout << "GET" << std::endl;
			print_arr(arr1);
			std::cout << "RIGHT" << std::endl;
			print_arr(arr2);
			return;
		}
	}
	std::cout << "RIGHT ANSWER!" << std::endl;
}


std::vector<int> Sort(const std::vector<int> &arr) {
	Sorter sort(arr);
	return sort.quick_sort();
}

int main() {
	std::cout << CONCURRENCY;
	if (CONCURRENCY == 1) {
		std::cout << " thread" << std::endl;
	} else {
		std::cout << " threads" << std::endl;
	}
	size_t arr_size = 1000000;
	std::vector<int> arr = generate(arr_size);
	std::vector<int> sorted = Sort(arr);
	test(sorted, arr);
	return 0;
}


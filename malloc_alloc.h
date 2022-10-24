#ifndef MALLOC_ALLOC
#define MALLOC_ALLOC

#if 0
# include<new>
# define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
# include<iostream>
# define THROW_BAD_ALLOC cerr<<"out of memory"<<endl;exit(1);
#endif

using std::cerr;
using std::endl;

namespace std {
	class malloc_alloc
	{
		//用于处理内存不足的情况的函数
	private:
		static void* oom_malloc(size_t);
		static void* oom_realloc(void*, size_t);
		static void(*malloc_alloc_oom_handler)();

	public:
		static void* allocate(size_t n);
		static void deallocate(void* p, size_t n);
		static void* reallocate(void* p, size_t size_old, size_t size_new);
		static void (*set_malloc_handler(void (*f)()))();
	};
}


#endif // !MALLOC_ALLOC



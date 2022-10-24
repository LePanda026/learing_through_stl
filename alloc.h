#ifndef ALLOC
#define ALLOC

#include<cstdlib>

namespace std {

	class alloc{
		//常量的定义
	private:
		enum EAlign { ALIGN = 8 };//小型区块的上调边界
		enum EMaxBytes { MAXBYTES = 128 };//小型区块的个数的上限，超过的区块由malloc分配
		enum ENFreeLists { NFREELISTS = EMaxBytes::MAXBYTES / EAlign::ALIGN };//free_lists的个数
	//free_lists
	private:
		//free_lists的节点构造
		union obj {
			union obj* next;
			char client_data[1];
		};

		//free_list数组
		static obj* free_lists[ENFreeLists::NFREELISTS];

		//内存池
	private:
		static char* start_free;
		static char* end_free;
		static size_t heap_size;

		//对free_lists进行操作的函数
		static size_t RoundUp(size_t bytes) {
			return ((bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1));
		}

		static size_t Index_Freelist(size_t bytes) {
			return ((bytes + EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
		}

		static void* refill(size_t n);

		static char* chunk_alloc(size_t size, int & nobjs);

	public:
		static void* allocate(size_t bytes);
		static void deallocate(void* ptr, size_t bytes);
		static void* reallocate(void* ptr, size_t size_old, size_t size_new);
	};
}
#endif // !ALLOC
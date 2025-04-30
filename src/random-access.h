#pragma once

#define DRAM_MAX_SIZE 1000000 /* 1MB */

// These are used for errors
#define DRAM_SIZE_TOO_LARGE 1
#define DRAM_ADDRESS_CACHE_FAIL_ALLOCATION 2
#define DRAM_ALLOCATOR_FAIL_ALLOCATION 3
#define DRAM_MEMORY_BLOCK_FAIL_ALLOCATION 4

namespace memory
{

	typedef unsigned long long llu;

	/// <summary>
	/// Creates a dummy ram module for safe allocation and deallocation.
	/// There is a max size of 1MB or 1000000b, Access that vairable using 'DRAM_MAX_SIZE'
	/// </summary>
	class dram
	{
	private:
		struct address
		{
			void* addr;
			llu offset;
			llu size;
		} null;

		address* m_addressCache;
		llu m_cacheSize;

		void* m_memoryBlock;
		bool* m_allocator;

		llu m_size;
	public:
		dram();
		dram(llu size);
		dram(dram& _ram);
		~dram();

		/// <summary>
		/// Allocates memory in the dummy ram
		/// </summary>
		/// <param name="size">: size of the memory block segment</param>
		/// <returns>Returns the void ptr to the memory block segment</returns>
		void* dmalloc(llu size);

		/// <summary>
		/// Unallocates memory in the dummy ram
		/// </summary>
		/// <param name="Address">: ptr to the memory block segment</param>
		void dfree(void* Address);
	};
}

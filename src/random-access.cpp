#include "random-access.h"

#include <iostream>
#include <stdlib.h>
#include <string.h>

memory::dram::dram() : null({nullptr, 0, 0}), m_addressCache(nullptr), m_cacheSize(0), m_memoryBlock(nullptr), m_allocator(nullptr), m_size(0) {}

memory::dram::dram(llu size)
	: null({ nullptr, 0, 0 }), m_addressCache(nullptr), m_cacheSize(0), m_memoryBlock(nullptr), m_allocator(nullptr), m_size(0)
{
	/// Catch if the size is too large
	if (size >= DRAM_MAX_SIZE)
	{
		std::cerr << "Error: [DRAM_SIZE_TOO_LARGE](" << DRAM_SIZE_TOO_LARGE << ") size is too large! your size: " << size << "b, max size: " << DRAM_MAX_SIZE << "b\n";
		exit(DRAM_SIZE_TOO_LARGE);
	}

	m_size = size;
	m_cacheSize = (llu)(m_size / 2);
	m_addressCache = (address*)malloc(m_cacheSize * sizeof(address));

	/// Catch if address cache isn't allocated
	if (!m_addressCache)
	{
		std::cerr << "Error: [DRAM_ADDRESS_CACHE_FAIL_ALLOCATION](" << DRAM_ADDRESS_CACHE_FAIL_ALLOCATION << ") Address cache failed to get allocated!\n";
		exit(DRAM_ADDRESS_CACHE_FAIL_ALLOCATION);
	}

	memset(m_addressCache, 0, (m_cacheSize * sizeof(address)));
	m_allocator = (bool*)malloc(m_size);

	/// Catch if allocator isn't allocated
	if (!m_allocator)
	{
		free(m_addressCache);

		std::cerr << "Error: [DRAM_ALLOCATOR_FAIL_ALLOCATION](" << DRAM_ALLOCATOR_FAIL_ALLOCATION << ") Allocator failed to get allocated!\n";
		exit(DRAM_ALLOCATOR_FAIL_ALLOCATION);
	}

	memset(m_allocator, 0, m_size);
	m_memoryBlock = malloc(m_size);

	/// Catch if memory block isn't allocated
	if (!m_memoryBlock)
	{
		free(m_addressCache);
		free(m_allocator);

		std::cerr << "Error: [DRAM_MEMORY_BLOCK_FAIL_ALLOCATION](" << DRAM_MEMORY_BLOCK_FAIL_ALLOCATION << ") Memory block failed to get allocated!\n";
		exit(DRAM_ALLOCATOR_FAIL_ALLOCATION);
	}
}

memory::dram::dram(dram& _ram)
	: null({ nullptr, 0, 0 }), m_addressCache(nullptr), m_cacheSize(0), m_memoryBlock(nullptr), m_allocator(nullptr), m_size(0)
{
	/// Catch if the size is too large
	if (_ram.m_size > DRAM_MAX_SIZE)
	{
		std::cerr << "Error: [DRAM_SIZE_TOO_LARGE](" << DRAM_SIZE_TOO_LARGE << ") size is too large! your size: " << _ram.m_size << "b, max size: " << DRAM_MAX_SIZE << "b\n";
		exit(DRAM_SIZE_TOO_LARGE);
	}

	m_size = _ram.m_size;
	m_cacheSize = (llu)(m_size / 2);
	m_addressCache = (address*)malloc(m_cacheSize * sizeof(address));

	/// Catch if address cache isn't allocated
	if (!m_addressCache)
	{
		std::cerr << "Error: [DRAM_ADDRESS_CACHE_FAIL_ALLOCATION](" << DRAM_ADDRESS_CACHE_FAIL_ALLOCATION << ") Address cache failed to get allocated!\n";
		exit(DRAM_ADDRESS_CACHE_FAIL_ALLOCATION);
	}

	memset(m_addressCache, 0, (m_cacheSize * sizeof(address)));
	m_allocator = (bool*)malloc(m_size);

	/// Catch if allocator isn't allocated
	if (!m_allocator)
	{
		free(m_addressCache);

		std::cerr << "Error: [DRAM_ALLOCATOR_FAIL_ALLOCATION](" << DRAM_ALLOCATOR_FAIL_ALLOCATION << ") Allocator failed to get allocated!\n";
		exit(DRAM_ALLOCATOR_FAIL_ALLOCATION);
	}

	memset(m_allocator, 0, m_size);
	m_memoryBlock = malloc(m_size);

	/// Catch if memory block isn't allocated
	if (!m_memoryBlock)
	{
		free(m_addressCache);
		free(m_allocator);

		std::cerr << "Error: [DRAM_MEMORY_BLOCK_FAIL_ALLOCATION](" << DRAM_MEMORY_BLOCK_FAIL_ALLOCATION << ") Memory block failed to get allocated!\n";
		exit(DRAM_ALLOCATOR_FAIL_ALLOCATION);
	}
}

memory::dram::~dram()
{
	/* Cleans up the dram */
	free(m_addressCache);
	free(m_allocator);
	free(m_memoryBlock);
}

void* memory::dram::dmalloc(llu size)
{
	/* if size is 0 */
	if (!size) return nullptr;

	/* Check through m_allocator to find unallocated memory */
	for (llu i = 0; i < m_size; i++)
	{
		/* if the current byte in m_allocator is unallocated meaning its 0 */
		if (!(*(m_allocator + i)))
		{
			int byteCount = 0;
			/* Count through every byte after the starting address to check if every byte after that is unallocated */
			for (llu j = 0; j < size; j++)
			{
				if (!(*(m_allocator + i + j))) byteCount++;
			}

			/* if the memory block segment is open, the program is going to cache the address along with the offset from the memory block start address and the size of the memory block segment */
			if (byteCount == size)
			{
				/* Check for an open cache sector */
				for (llu j = 0; j < size; j++)
				{
					/* If the cache sector is null then it is going to cache the address along with the offset from the memory block start address and the size of the memory block segment */
					if (!(*(m_addressCache + j)).addr)
					{
						address a = { (void*)((char*)m_memoryBlock + i), i, size };
						*(m_addressCache + j) = a;
						break;
					}
				}

				/* Set the memory block segment in m_allocator to allocated so no other program uses it */
				for (llu j = 0; j < size; j++)
				{
 					if (!(*(m_allocator + i + j))) (*(m_allocator + i + j)) = 1;
				}

				/* return the allocated memory aaddress */
				return (void*)((char*)m_memoryBlock + i);
			}
		}
	}

	/* return a nullptr if dram has not memory left */
	return nullptr;
}

void memory::dram::dfree(void* Address)
{
	/* If Address is 0, cannot free a nullptr */
	if (!Address) return;

	/* Look through the address cache to find the memory block. If it doen't find the cached address it will just return */
	for (llu i = 0; i < m_cacheSize; i++)
	{
		/* If Address is equal to the cached address */
		if ((*(m_addressCache + i)).addr == Address)
		{
			/* Unallocates allocated memory */
			for (llu j = 0; j < (*(m_addressCache + i)).size; j++)
			{
				*(((char*)m_allocator + (*(m_addressCache + i)).offset) + j) = 0;
			}

			/* Uncache the address */
			*(m_addressCache + i) = null;
			break;
		}
	}
}

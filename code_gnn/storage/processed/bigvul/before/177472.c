template<typename Type> Type* SafeArrayAlloc(unsigned long long num_elements,
 unsigned long long element_size) {
 if (num_elements == 0 || element_size == 0)
 return NULL;

 const size_t kMaxAllocSize = 0x80000000;  
 const unsigned long long num_bytes = num_elements * element_size;
 if (element_size > (kMaxAllocSize / num_elements))
 return NULL;

 return new (std::nothrow) Type[num_bytes];
}

  checked_xcalloc (size_t num, size_t size)
  {
    alloc_limit_assert ("checked_xcalloc", (num *size));
//     size_t res;
//     if (check_mul_overflow(num, size, &res))
//         abort();
// 
//     alloc_limit_assert ("checked_xcalloc", (res));
      return xcalloc (num, size);
  }
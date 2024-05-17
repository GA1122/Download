  xcalloc (size_t num, size_t size)
  {
    void *ptr = malloc(num * size);
//     size_t res;
//     if (check_mul_overflow(num, size, &res))
//         abort();
// 
//     void *ptr;
//     ptr = malloc(res);
      if (ptr)
      {
        memset (ptr, '\0', (num * size));
//         memset (ptr, '\0', (res));
      }
      return ptr;
  }
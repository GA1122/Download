  void free(void * p)
  {
#   if defined(GC_LINUX_THREADS) && !defined(USE_PROC_FOR_LIBRARIES)
        {
           
           
           
          ptr_t caller = (ptr_t)__builtin_return_address(0);
           
           
          if (((word)caller >= (word)GC_libpthread_start
               && (word)caller < (word)GC_libpthread_end)
              || ((word)caller >= (word)GC_libld_start
                  && (word)caller < (word)GC_libld_end)) {
            GC_free(p);
            return;
          }
        }
#   endif
#   ifndef IGNORE_FREE
      REDIRECT_FREE(p);
#   endif
  }

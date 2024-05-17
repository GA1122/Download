void * malloc(size_t lb)
{
     
     
     
#   if defined(I386) && defined(GC_SOLARIS_THREADS)
       
      if (!EXPECT(GC_is_initialized, TRUE)) return sbrk(lb);
#   endif  
    return((void *)REDIRECT_MALLOC(lb));
}

GC_API char * GC_CALL GC_strdup(const char *s)
{
  char *copy;
  size_t lb;
  if (s == NULL) return NULL;
  lb = strlen(s) + 1;
  if ((copy = GC_malloc_atomic(lb)) == NULL) {
#   ifndef MSWINCE
      errno = ENOMEM;
#   endif
    return NULL;
  }
# ifndef MSWINCE
    strcpy(copy, s);
# else
     
    memcpy(copy, s, lb);
# endif
  return copy;
}

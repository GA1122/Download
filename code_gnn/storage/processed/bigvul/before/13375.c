Xrealloc(void *ptr, unsigned long amount)
{
     
    if ((long)amount <= 0)
	ErrorF("Warning: Xrealloc: "
	       "requesting unpleasantly large amount of memory: %lu bytes.\n",
               amount);

    return realloc(ptr, amount);
}

Xalloc(unsigned long amount)
{
     
    if ((long)amount <= 0)
	ErrorF("Warning: Xalloc: "
	       "requesting unpleasantly large amount of memory: %lu bytes.\n",
               amount);

    return malloc(amount);
}

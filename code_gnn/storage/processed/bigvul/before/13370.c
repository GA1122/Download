XNFrealloc(void *ptr, unsigned long amount)
{
    void *ret = realloc(ptr, amount);
    if (!ret)
	FatalError("XNFrealloc: Out of memory");
    return ret;
}

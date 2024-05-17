random_level (void)
{
     
    long int	bits = FcRandom () | FcRandom ();
    int	level = 0;

    while (++level < FC_CACHE_MAX_LEVEL)
    {
	if (bits & 1)
	    break;
	bits >>= 1;
    }
    return level;
}

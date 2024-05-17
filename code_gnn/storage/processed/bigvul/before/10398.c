FcDirCacheBasename (const FcChar8 * dir, FcChar8 cache_base[CACHEBASE_LEN])
{
    unsigned char 	hash[16];
    FcChar8		*hex_hash;
    int			cnt;
    struct MD5Context 	ctx;

    MD5Init (&ctx);
    MD5Update (&ctx, (const unsigned char *)dir, strlen ((const char *) dir));

    MD5Final (hash, &ctx);

    cache_base[0] = '/';
    hex_hash = cache_base + 1;
    for (cnt = 0; cnt < 16; ++cnt)
    {
	hex_hash[2*cnt  ] = bin2hex[hash[cnt] >> 4];
	hex_hash[2*cnt+1] = bin2hex[hash[cnt] & 0xf];
    }
    hex_hash[2*cnt] = 0;
    strcat ((char *) cache_base, "-" FC_ARCHITECTURE FC_CACHE_SUFFIX);

    return cache_base;
}

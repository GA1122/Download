fbCombineMaskC (CARD32 *src, CARD32 *mask)
{
    CARD32 a = *mask;

    CARD32	x;
    CARD16	xa;

    if (!a)
    {
	WRITE(src, 0);
	return;
    }

    x = READ(src);
    if (a == 0xffffffff)
    {
	x = x >> 24;
	x |= x << 8;
	x |= x << 16;
	WRITE(mask, x);
	return;
    }

    xa = x >> 24;
    FbByteMulC(x, a);
    WRITE(src, x);
    FbByteMul(a, xa);
    WRITE(mask, a);
}

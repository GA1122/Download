RenderSetBit (unsigned char *line, int x, int bit)
{
    unsigned char   mask;
    
    if (screenInfo.bitmapBitOrder == LSBFirst)
	mask = (1 << (x & 7));
    else
	mask = (0x80 >> (x & 7));
     
    line += (x >> 3);
    if (bit)
	*line |= mask;
    else
	*line &= ~mask;
}

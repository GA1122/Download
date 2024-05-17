GetBit(unsigned char *line, int x)
{
    unsigned char mask;

    if (screenInfo.bitmapBitOrder == LSBFirst)
        mask = (1 << (x & 7));
    else
        mask = (0x80 >> (x & 7));
     
    line += (x >> 3);
    if (*line & mask)
        return 1;
    return 0;
}

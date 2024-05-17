CopyBits(char *dst, int shiftL, char *src, int bytes)
{
     
    int shiftR = 8 - shiftL;

    while (bytes--) {
        *dst |= SHIFT_L(*src, shiftL);
        *(dst + 1) |= SHIFT_R(*src, shiftR);
        dst++;
        src++;
    }
}

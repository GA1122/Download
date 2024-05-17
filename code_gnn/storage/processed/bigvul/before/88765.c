size_t strlcpy(char *dest, const char *src, size_t dest_size)
{
    register char *d = dest;
    register const char *s = src;
    register size_t n = dest_size;

     
    if (n != 0 && --n != 0) {
        do {
            if ((*d++ = *s++) == 0)
                break;
        } while (--n != 0);
    }

     
    if (n == 0) {
        if (dest_size != 0)
            *d = '\0';  
        while (*s++)
            ;
    }

    return (s - src - 1);  
}

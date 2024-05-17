static inline void bd_copy(UINT8* dest, UINT8* src, BOOLEAN swap)
{
 if (swap) {
 int i;
 for (i =0; i < 6 ;i++)
            dest[i]= src[5-i];
 }
 else memcpy(dest, src, 6);
}

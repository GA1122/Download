 byteSwap(UWORD32 *buf, unsigned words) {
   md5byte *p;
 
  
 int i = 1;

 if (*(char *)&i == 1)
 return;

  p = (md5byte *)buf;

 do {
 *buf++ = (UWORD32)((unsigned)p[3] << 8 | p[2]) << 16 |
 ((unsigned)p[1] << 8 | p[0]);
    p += 4;
 } while (--words);
}

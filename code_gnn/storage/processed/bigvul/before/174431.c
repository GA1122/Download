static int parseExcludedChannels( UINT *excludedChnsMask,
                                  HANDLE_FDK_BITSTREAM bs )
{
  UINT excludeMask = 0;
  UINT i, j;
 int  bitCnt = 9;

 for (i = 0, j = 1; i < 7; i++, j<<=1) {
 if (FDKreadBits(bs,1)) {
      excludeMask |= j;
 }
 }

  
 while (FDKreadBits(bs,1)) {
 for (i = 0; i < 7; i++, j<<=1) {
 if (FDKreadBits(bs,1)) {
        excludeMask |= j;
 }
 }
    bitCnt += 9;
    FDK_ASSERT(j < (UINT)-1);
 }

 *excludedChnsMask = excludeMask;

 return (bitCnt);
}

static void adjustStats(int iSize, int increment){
  int i = ROUND8(iSize)/8;
  if( i>NCSIZE-1 ){
    i = NCSIZE - 1;
  }
  if( increment>0 ){
    mem.nAlloc[i]++;
    mem.nCurrent[i]++;
    if( mem.nCurrent[i]>mem.mxCurrent[i] ){
      mem.mxCurrent[i] = mem.nCurrent[i];
    }
  }else{
    mem.nCurrent[i]--;
    assert( mem.nCurrent[i]>=0 );
  }
}

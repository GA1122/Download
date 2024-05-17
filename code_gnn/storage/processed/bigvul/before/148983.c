static void asciiFold(char *aOut, const char *aIn, int nByte){
  int i;
  for(i=0; i<nByte; i++){
    char c = aIn[i];
    if( c>='A' && c<='Z' ) c += 32;
    aOut[i] = c;
  }
}

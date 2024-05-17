static int analyzeOverKeyword(const unsigned char *z, int lastToken){
  if( lastToken==TK_RP ){
    int t = getToken(&z);
    if( t==TK_LP || t==TK_ID ) return TK_OVER;
  }
  return TK_ID;
}

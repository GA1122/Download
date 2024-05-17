static int analyzeFilterKeyword(const unsigned char *z, int lastToken){
  if( lastToken==TK_RP && getToken(&z)==TK_LP ){
    return TK_FILTER;
  }
  return TK_ID;
}

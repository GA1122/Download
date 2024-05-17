static int analyzeWindowKeyword(const unsigned char *z){
  int t;
  t = getToken(&z);
  if( t!=TK_ID ) return TK_ID;
  t = getToken(&z);
  if( t!=TK_AS ) return TK_ID;
  return TK_WINDOW;
}

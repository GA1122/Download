static void datetimeError(DateTime *p){
  memset(p, 0, sizeof(*p));
  p->isError = 1;
}

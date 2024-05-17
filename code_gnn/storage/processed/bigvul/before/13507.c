int inflate()
 
{
  int e;                 
  int r;                 
  unsigned h;            


   
  wp = 0;
  bk = 0;
  bb = 0;


   
  h = 0;
  do {
    hufts = 0;
    if ((r = inflate_block(&e)) != 0)
      return r;
    if (hufts > h)
      h = hufts;
  } while (!e);

   
  while (bk >= 8) {
    bk -= 8;
    inptr--;
  }

   
  flush_output(wp);


   
  Trace ((stderr, "<%u> ", h));
  return 0;
}

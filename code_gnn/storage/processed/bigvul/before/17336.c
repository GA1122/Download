brand ()
{
   
  long h, l;

   
  if (rseed == 0)
    rseed = 123459876;
  h = rseed / 127773;
  l = rseed % 127773;
  rseed = 16807 * l - 2836 * h;
#if 0
  if (rseed < 0)
    rseed += 0x7fffffff;
#endif
  return ((unsigned int)(rseed & 32767));	 
}

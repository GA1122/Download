g_unicode_canonical_ordering (gunichar * string, gsize len)
{
  gsize i;
  int swap = 1;

  while (swap)
    {
      int last;
      swap = 0;
      last = COMBINING_CLASS (string[0]);
      for (i = 0; i < len - 1; ++i)
	{
	  int next = COMBINING_CLASS (string[i + 1]);
	  if (next != 0 && last > next)
	    {
	      gsize j;
	       
	      for (j = i + 1; j > 0; --j)
		{
		  gunichar t;
		  if (COMBINING_CLASS (string[j - 1]) <= next)
		    break;
		  t = string[j];
		  string[j] = string[j - 1];
		  string[j - 1] = t;
		  swap = 1;
		}
	       
	      next = last;
	    }
	  last = next;
	}
    }
}

locale_setblanks ()
{
  int x;

  for (x = 0; x < sh_syntabsiz; x++)
    {
      if (isblank (x))
	sh_syntaxtab[x] |= CSHBRK|CBLANK;
      else if (member (x, shell_break_chars))
	{
	  sh_syntaxtab[x] |= CSHBRK;
	  sh_syntaxtab[x] &= ~CBLANK;
	}
      else
	sh_syntaxtab[x] &= ~(CSHBRK|CBLANK);
    }
}

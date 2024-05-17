SelectRendition()
{
#ifdef COLOR
  register int j, i = 0, a = curr->w_rend.attr, c = curr->w_rend.color;
# ifdef COLORS256
  int cx = curr->w_rend.colorx;
# endif
#else
  register int j, i = 0, a = curr->w_rend.attr;
#endif

  do
    {
      j = curr->w_args[i];
#ifdef COLOR
      if ((j == 38 || j == 48) && i + 2 < curr->w_NumArgs && curr->w_args[i + 1] == 5)
	{
	  int jj;

	  i += 2;
	  jj = curr->w_args[i];
	  if (jj < 0 || jj > 255)
	    continue;
# ifdef COLORS256
	  if (j == 38)
	    {
	      c = (c & 0xf0) | ((jj & 0x0f) ^ 9);
	      a |= A_BFG;
	      if (jj >= 8 && jj < 16)
		c |= 0x08;
	      else
		a ^= A_BFG;
	      a = (a & 0xbf) | (jj & 8 ? 0x40 : 0);
	      cx = (cx & 0xf0) | (jj >> 4 & 0x0f);
	    }
	  else
	    {
	      c = (c & 0x0f) | ((jj & 0x0f) ^ 9) << 4;
	      a |= A_BBG;
	      if (jj >= 8 && jj < 16)
		c |= 0x80;
	      else
		a ^= A_BBG;
	      cx = (cx & 0x0f) | (jj & 0xf0);
	    }
	  continue;
# else
	  jj = color256to16(jj) + 30;
	  if (jj >= 38)
	    jj += 60 - 8;
	  j = j == 38 ? jj : jj + 10;
# endif
	}
# ifdef COLORS16
      if (j == 0 || (j >= 30 && j <= 39 && j != 38))
	a &= 0xbf;
      if (j == 0 || (j >= 40 && j <= 49 && j != 48))
	a &= 0x7f;
      if (j >= 90 && j <= 97)
	a |= 0x40;
      if (j >= 100 && j <= 107)
	a |= 0x80;
# endif
      if (j >= 90 && j <= 97)
	j -= 60;
      if (j >= 100 && j <= 107)
	j -= 60;
      if (j >= 30 && j <= 39 && j != 38)
	c = (c & 0xf0) | ((j - 30) ^ 9);
      else if (j >= 40 && j <= 49 && j != 48)
	c = (c & 0x0f) | (((j - 40) ^ 9) << 4);
      if (j == 0)
	c = 0;
# ifdef COLORS256
      if (j == 0 || (j >= 30 && j <= 39 && j != 38))
	cx &= 0xf0;
      if (j == 0 || (j >= 40 && j <= 49 && j != 48))
	cx &= 0x0f;
# endif
#endif
      if (j < 0 || j >= (int)(sizeof(rendlist)/sizeof(*rendlist)))
	continue;
      j = rendlist[j];
      if (j & (1 << NATTR))
        a &= j;
      else
        a |= j;
    }
  while (++i < curr->w_NumArgs);
  curr->w_rend.attr = a;
#ifdef COLOR
  curr->w_rend.color = c;
# ifdef COLORS256
  curr->w_rend.colorx = cx;
# endif
#endif
  LSetRendition(&curr->w_layer, &curr->w_rend);
}

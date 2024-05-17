pch_normalize (enum diff format)
{
  lin old = 1;
  lin new = p_ptrn_lines + 1;

  while (p_Char[new] == '=' || p_Char[new] == '\n')
    new++;

  if (format == UNI_DIFF)
    {
       

      for (; old <= p_ptrn_lines; old++)
	if (p_Char[old] == '!')
	  p_Char[old] = '-';
      for (; new <= p_end; new++)
	if (p_Char[new] == '!')
	  p_Char[new] = '+';
    }
  else
    {
       

      while (old <= p_ptrn_lines)
	{
	  if (p_Char[old] == '-')
	    {
	      if (new <= p_end && p_Char[new] == '+')
		{
		  do
		    {
		      p_Char[old] = '!';
		      old++;
		    }
		  while (old <= p_ptrn_lines && p_Char[old] == '-');
		  do
		    {
		      p_Char[new] = '!';
		      new++;
		    }
		  while (new <= p_end && p_Char[new] == '+');
		}
	      else
		{
		  do
		    old++;
		  while (old <= p_ptrn_lines && p_Char[old] == '-');
		}
	    }
	  else if (new <= p_end && p_Char[new] == '+')
	    {
	      do
		new++;
	      while (new <= p_end && p_Char[new] == '+');
	    }
	  else
	    {
	      old++;
	      new++;
	    }
	}
    }
}

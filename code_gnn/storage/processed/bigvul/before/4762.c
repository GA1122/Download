combine (gunichar a, gunichar b, gunichar * result)
{
  gushort index_a, index_b;

  if (combine_hangul (a, b, result))
    return TRUE;

  index_a = COMPOSE_INDEX (a);

  if (index_a >= COMPOSE_FIRST_SINGLE_START && index_a < COMPOSE_SECOND_START)
    {
      if (b == compose_first_single[index_a - COMPOSE_FIRST_SINGLE_START][0])
	{
	  *result =
	    compose_first_single[index_a - COMPOSE_FIRST_SINGLE_START][1];
	  return TRUE;
	}
      else
	return FALSE;
    }

  index_b = COMPOSE_INDEX (b);

  if (index_b >= COMPOSE_SECOND_SINGLE_START)
    {
      if (a ==
	  compose_second_single[index_b - COMPOSE_SECOND_SINGLE_START][0])
	{
	  *result =
	    compose_second_single[index_b - COMPOSE_SECOND_SINGLE_START][1];
	  return TRUE;
	}
      else
	return FALSE;
    }

  if (index_a >= COMPOSE_FIRST_START && index_a < COMPOSE_FIRST_SINGLE_START
      && index_b >= COMPOSE_SECOND_START
      && index_b < COMPOSE_SECOND_SINGLE_START)
    {
      gunichar res =
	compose_array[index_a - COMPOSE_FIRST_START][index_b -
						     COMPOSE_SECOND_START];

      if (res)
	{
	  *result = res;
	  return TRUE;
	}
    }

  return FALSE;
}

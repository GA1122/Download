decompose_hangul (gunichar s, gunichar * r, gsize * result_len)
{
  gint SIndex = s - SBase;
  gint TIndex = SIndex % TCount;

  if (r)
    {
      r[0] = LBase + SIndex / NCount;
      r[1] = VBase + (SIndex % NCount) / TCount;
    }

  if (TIndex)
    {
      if (r)
	r[2] = TBase + TIndex;
      *result_len = 3;
    }
  else
    *result_len = 2;
}

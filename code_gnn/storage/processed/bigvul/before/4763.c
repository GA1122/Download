combine_hangul (gunichar a, gunichar b, gunichar * result)
{
  gint LIndex = a - LBase;
  gint SIndex = a - SBase;

  gint VIndex = b - VBase;
  gint TIndex = b - TBase;

  if (0 <= LIndex && LIndex < LCount && 0 <= VIndex && VIndex < VCount)
    {
      *result = SBase + (LIndex * VCount + VIndex) * TCount;
      return TRUE;
    }
  else if (0 <= SIndex && SIndex < SCount && (SIndex % TCount) == 0
	   && 0 < TIndex && TIndex < TCount)
    {
      *result = a + TIndex;
      return TRUE;
    }

  return FALSE;
}

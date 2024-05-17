decimal_point_char (void)
{
  const char *point;
   
#  if HAVE_NL_LANGINFO && (__GLIBC__ || defined __UCLIBC__ || (defined __APPLE__ && defined __MACH__))
  point = nl_langinfo (RADIXCHAR);
#  elif 1
  char pointbuf[5];
  sprintf (pointbuf, "%#.0f", 1.0);
  point = &pointbuf[1];
#  else
  point = localeconv () -> decimal_point;
#  endif
   
  return (point[0] != '\0' ? point[0] : '.');
}

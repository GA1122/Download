static void get_charstring_token(void)
{
  int c = getc(ifp);
  while (isspace(c))
    c = getc(ifp);

  if (c == '%') {
    while (c != EOF && c != '\r' && c != '\n')
      c = getc(ifp);
    get_charstring_token();

  } else if (c == '}') {
    line[0] = '}';

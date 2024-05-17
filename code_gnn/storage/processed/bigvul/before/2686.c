incomplete_line (void)
{
  FILE *fp = pfp;
  int c;
  file_offset line_beginning = file_tell (fp);

  if (getc (fp) == '\\')
    {
      while ((c = getc (fp)) != '\n'  &&  c != EOF)
	  ;
      return true;
    }
  else
    {
       
      Fseek (pfp, line_beginning, SEEK_SET);
      return false;
    }
}

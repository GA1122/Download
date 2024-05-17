static int jpeg_skip_till_marker(Image *ifile, int marker)
{
  int c, i;

  do
  {
     
    i = 0;
    do
    {
      c = ReadBlobByte(ifile);
      i++;
      if (c == EOF)
        return M_EOI;  
    } while (c != 0xff);

     
    do
    {
      c = ReadBlobByte(ifile);
      if (c == EOF)
        return M_EOI;  
    } while (c == 0xff);
  } while (c != marker);
  return c;
}

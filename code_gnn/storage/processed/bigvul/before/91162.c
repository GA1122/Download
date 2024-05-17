static int jpeg_nextmarker(Image *ifile, Image *ofile)
{
  int c;

   
  do
  {
    c = ReadBlobByte(ifile);
    if (c == EOF)
      return M_EOI;  
    else
      if (c != 0xff)
        (void) WriteBlobByte(ofile,(unsigned char) c);
  } while (c != 0xff);

   
  do
  {
    c = ReadBlobByte(ifile);
    if (c == EOF)
      return M_EOI;  
  } while (c == 0xff);

  return c;
}

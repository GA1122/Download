ushort CLASS sget2 (uchar *s)
{
  if (order == 0x4949)		 
    return s[0] | s[1] << 8;
  else				 
    return s[0] << 8 | s[1];
}

static byte cencrypt(byte plain)
{
  byte cipher;

   
  if (lenIV < 0) return plain;

  cipher = (byte)(plain ^ (cr >> 8));
  cr = (uint16_t)((cipher + cr) * c1 + c2);
  return cipher;
}

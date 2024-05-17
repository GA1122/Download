unsigned getValueRequiredBits(unsigned char value)
{
  if(value == 0 || value == 255) return 1;
   
  if(value % 17 == 0) return value % 85 == 0 ? 2 : 4;
  return 8;
}

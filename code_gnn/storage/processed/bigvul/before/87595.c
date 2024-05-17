static void setBitOfReversedStream(size_t* bitpointer, unsigned char* bitstream, unsigned char bit)
{
   
  if(bit == 0)
  {
    size_t pos = (*bitpointer) >> 3;
    bitstream[pos] &=  (unsigned char)(~(1 << (7 - ((*bitpointer) & 0x7))));
  }
  else
  {
    size_t pos = (*bitpointer) >> 3;
    bitstream[pos] |=  (1 << (7 - ((*bitpointer) & 0x7)));
  }
  (*bitpointer)++;
}
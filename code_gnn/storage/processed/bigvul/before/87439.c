static int addBitToStream(size_t* bitpointer, ucvector* bitstream, unsigned char bit)
{
   
  if(((*bitpointer) & 7) == 0)
  {
    if (!ucvector_push_back(bitstream, (unsigned char)0)) return 83;
  }

   
  (bitstream->data[bitstream->size - 1]) |= (bit << ((*bitpointer) & 0x7));
  (*bitpointer)++;

  return 0;
}

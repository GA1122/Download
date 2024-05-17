unsigned lodepng_chunk_create(unsigned char** out, size_t* outlength, unsigned length,
                              const char* type, const unsigned char* data)
{
  unsigned i;
  unsigned char *chunk, *new_buffer;
  size_t new_length = (*outlength) + length + 12;
  if(new_length < length + 12 || new_length < (*outlength)) return 77;  
  new_buffer = (unsigned char*)realloc(*out, new_length);
  if(!new_buffer) return 83;  
  (*out) = new_buffer;
  (*outlength) = new_length;
  chunk = &(*out)[(*outlength) - length - 12];

   
  lodepng_set32bitInt(chunk, (unsigned)length);

   
  chunk[4] = (unsigned char)type[0];
  chunk[5] = (unsigned char)type[1];
  chunk[6] = (unsigned char)type[2];
  chunk[7] = (unsigned char)type[3];

   
  for(i = 0; i < length; i++) chunk[8 + i] = data[i];

   
  lodepng_chunk_generate_crc(chunk);

  return 0;
}

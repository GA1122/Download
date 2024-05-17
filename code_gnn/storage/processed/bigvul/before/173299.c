get32(png_bytep buffer, int offset)
  
{
 return
 (buffer[ offset    & 7] << 24) +
 (buffer[(offset+1) & 7] << 16) +
 (buffer[(offset+2) & 7] << 8) +
 (buffer[(offset+3) & 7] );
}

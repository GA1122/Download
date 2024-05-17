file_write(char *filename, char *data)
{
  FILE*       fp;
  int         bytes_written = 0;
  int         len           = strlen(data);  

  if (!(fp = fopen(filename, "wb"))) 
    return -1;
 
  while ( len > 0 )
    {
      bytes_written = fwrite(data, sizeof(char), len, fp);
      len = len - bytes_written;
      data = data + bytes_written;
    }

  fclose(fp);

  return 1;
}
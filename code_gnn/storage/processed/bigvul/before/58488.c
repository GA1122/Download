file_read(char *filename)
{
  struct stat st;
  FILE*       fp;
  char*       str;
  int         len;

   
  if (stat(filename, &st)) 
    return NULL;

  if (!(fp = fopen(filename, "rb"))) 
    return NULL;
  
  str = (char *)malloc(sizeof(char)*(st.st_size + 1));
  len = fread(str, 1, st.st_size, fp);
  if (len >= 0) str[len] = '\0';
  
  fclose(fp);

  return str;
}
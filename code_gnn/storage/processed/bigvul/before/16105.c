LoadData (const char * file_name, void *& data, int & data_size) {
  priv_state priv = set_root_priv();
  
  int fd = safe_open_wrapper_follow(file_name, O_RDONLY);
  if (fd == -1) {
    fprintf (stderr, "Can't open %s\n", file_name);
    set_priv (priv);
    return FALSE;
  }
  
  char buff [MAX_CRED_DATA_SIZE+1];
  data_size = read (fd, buff, MAX_CRED_DATA_SIZE);

  close (fd);

  if (data_size <= 0) {
    set_priv (priv);
    return FALSE;
  }

  data = malloc (data_size);

  memcpy (data, buff, data_size);

  set_priv (priv);
  return TRUE;

}

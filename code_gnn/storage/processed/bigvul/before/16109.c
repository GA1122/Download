StoreData (const char * file_name, const void * data, const int data_size) {

  if (!data) {
    return FALSE;
  }


  priv_state priv = set_root_priv();
  dprintf (D_FULLDEBUG, "in StoreData(), euid=%d\n", geteuid());

  int fd = safe_open_wrapper_follow(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600 );
  if (fd == -1) {
    dprintf (D_ALWAYS, "Unable to store in %s\n", file_name);
    set_priv(priv);
    return FALSE;
  }

  fchmod (fd, S_IRUSR | S_IWUSR);
  fchown (fd, get_user_uid(), get_user_gid());

  write (fd, data, data_size);

  close (fd);

  set_priv(priv);
  return TRUE;
}

get_user_challenge_file(YK_KEY *yk, const char *chalresp_path, const struct passwd *user, char **fn, FILE *debug_file)
{
   

   

  const char *filename = NULL;  
  char *ptr = NULL;
  unsigned int serial = 0;
  int ret;

  if (! yk_get_serial(yk, 0, 0, &serial)) {
    D (debug_file, "Failed to read serial number (serial-api-visible disabled?).");
    if (! chalresp_path)
      filename = "challenge";
    else
      filename = user->pw_name;
  } else {
     
     
    size_t len = strlen(chalresp_path == NULL ? "challenge" : user->pw_name) + 1 + 10 + 1;
    if ((ptr = malloc(len)) != NULL) {
      int res = snprintf(ptr, len, "%s-%u", chalresp_path == NULL ? "challenge" : user->pw_name, serial);
      filename = ptr;
      if (res < 0 || (unsigned long)res > len) {
	 
	free(ptr);
	filename = NULL;
      }
    }
  }

  if (filename == NULL)
    return 0;

  ret = get_user_cfgfile_path (chalresp_path, filename, user, fn);
  if(ptr) {
    free(ptr);
  }
  return ret;
}

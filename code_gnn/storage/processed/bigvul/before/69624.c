rend_service_load_keys(rend_service_t *s)
{
  char *fname = NULL;
  char buf[128];

   
  if (BUG(rend_service_check_private_dir(get_options(), s, 0) < 0))
    goto err;

   
  fname = rend_service_path(s, private_key_fname);
  s->private_key = init_key_from_file(fname, 1, LOG_ERR, 0);

  if (!s->private_key)
    goto err;

  if (rend_service_derive_key_digests(s) < 0)
    goto err;

  tor_free(fname);
   
  fname = rend_service_path(s, hostname_fname);

  tor_snprintf(buf, sizeof(buf),"%s.onion\n", s->service_id);
  if (write_str_to_file(fname,buf,0)<0) {
    log_warn(LD_CONFIG, "Could not write onion address to hostname file.");
    goto err;
  }
#ifndef _WIN32
  if (s->dir_group_readable) {
     
    if (chmod(fname, 0640))
      log_warn(LD_FS,"Unable to make hidden hostname file %s group-readable.",
               fname);
  }
#endif

   
  if (s->auth_type != REND_NO_AUTH) {
    if (rend_service_load_auth_keys(s, fname) < 0) {
      goto err;
    }
  }

  int r = 0;
  goto done;
 err:
  r = -1;
 done:
  memwipe(buf, 0, sizeof(buf));
  tor_free(fname);
  return r;
}

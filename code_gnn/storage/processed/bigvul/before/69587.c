poison_new_single_onion_hidden_service_dir_impl(const rend_service_t *service,
                                                const or_options_t* options)
{
   
  if (BUG(!service)) {
    return -1;
  }

   
  tor_assert(rend_service_non_anonymous_mode_enabled(options));

  int fd;
  int retval = -1;
  char *poison_fname = NULL;

  if (rend_service_is_ephemeral(service)) {
    log_info(LD_REND, "Ephemeral HS started in non-anonymous mode.");
    return 0;
  }

   
  if (rend_service_private_key_exists(service)) {
    log_warn(LD_BUG, "Tried to single onion poison a service directory after "
             "the private key was created.");
    return -1;
  }

   
  if (BUG(rend_service_check_private_dir_impl(options, service, 0) < 0))
    return -1;

  poison_fname = rend_service_sos_poison_path(service);

  switch (file_status(poison_fname)) {
  case FN_DIR:
  case FN_ERROR:
    log_warn(LD_FS, "Can't read single onion poison file \"%s\"",
             poison_fname);
    goto done;
  case FN_FILE:  
  case FN_EMPTY:  
    log_debug(LD_FS, "Tried to re-poison a single onion poisoned file \"%s\"",
              poison_fname);
    break;
  case FN_NOENT:
    fd = tor_open_cloexec(poison_fname, O_RDWR|O_CREAT|O_TRUNC, 0600);
    if (fd < 0) {
      log_warn(LD_FS, "Could not create single onion poison file %s",
               poison_fname);
      goto done;
    }
    close(fd);
    break;
  default:
    tor_assert(0);
  }

  retval = 0;

 done:
  tor_free(poison_fname);

  return retval;
}

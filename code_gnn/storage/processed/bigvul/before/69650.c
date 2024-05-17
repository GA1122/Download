service_is_single_onion_poisoned(const rend_service_t *service)
{
  char *poison_fname = NULL;
  file_status_t fstatus;

   
  if (BUG(!service)) {
    return 0;
  }

  if (rend_service_is_ephemeral(service)) {
    return 0;
  }

  poison_fname = rend_service_sos_poison_path(service);

  fstatus = file_status(poison_fname);
  tor_free(poison_fname);

   
  if (fstatus == FN_FILE || fstatus == FN_EMPTY) {
    return 1;
  }

  return 0;
}

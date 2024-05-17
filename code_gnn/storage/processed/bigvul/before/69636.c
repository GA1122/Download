rend_service_private_key_exists(const rend_service_t *service)
{
  char *private_key_path = rend_service_path(service, private_key_fname);
  const file_status_t private_key_status = file_status(private_key_path);
  tor_free(private_key_path);
   
  return private_key_status == FN_FILE;
}

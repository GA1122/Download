rend_service_check_dir_and_add(smartlist_t *service_list,
                               const or_options_t *options,
                               rend_service_t *service,
                               int validate_only)
{
  if (!service) {
     
    return 0;
  }

  if (rend_service_check_private_dir(options, service, !validate_only)
      < 0) {
    rend_service_free(service);
    return -1;
  }

  smartlist_t *s_list = rend_get_service_list_mutable(service_list);
   
  if (BUG(!s_list)) {
    return -1;
  }
  return rend_add_service(s_list, service);
}

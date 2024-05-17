rend_consider_services_upload(time_t now)
{
  int i;
  rend_service_t *service;
  const or_options_t *options = get_options();
  int rendpostperiod = options->RendPostPeriod;
  int rendinitialpostdelay = (options->TestingTorNetwork ?
                              MIN_REND_INITIAL_POST_DELAY_TESTING :
                              MIN_REND_INITIAL_POST_DELAY);

  for (i=0; i < smartlist_len(rend_service_list); ++i) {
    service = smartlist_get(rend_service_list, i);
    if (!service->next_upload_time) {  
       
      service->next_upload_time =
        now + rendinitialpostdelay + crypto_rand_int(2*rendpostperiod);
       
      if (rend_service_reveal_startup_time(options)) {
        service->next_upload_time = now + rendinitialpostdelay;
      }
    }
     
    unsigned int intro_points_ready =
      count_established_intro_points(service) >=
        service->n_intro_points_wanted;
    if (intro_points_ready &&
        (service->next_upload_time < now ||
        (service->desc_is_dirty &&
         service->desc_is_dirty < now-rendinitialpostdelay))) {
       
      rend_service_update_descriptor(service);
      upload_service_descriptor(service);
    }
  }
}

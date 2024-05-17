rend_service_update_descriptor(rend_service_t *service)
{
  rend_service_descriptor_t *d;
  origin_circuit_t *circ;
  int i;

  rend_service_descriptor_free(service->desc);
  service->desc = NULL;

  d = service->desc = tor_malloc_zero(sizeof(rend_service_descriptor_t));
  d->pk = crypto_pk_dup_key(service->private_key);
  d->timestamp = time(NULL);
  d->timestamp -= d->timestamp % 3600;  
  d->intro_nodes = smartlist_new();
   
  d->protocols = (1 << 2) + (1 << 3);

  for (i = 0; i < smartlist_len(service->intro_nodes); ++i) {
    rend_intro_point_t *intro_svc = smartlist_get(service->intro_nodes, i);
    rend_intro_point_t *intro_desc;

     
    intro_svc->listed_in_last_desc = 0;

    circ = find_intro_circuit(intro_svc, service->pk_digest);
    if (!circ || circ->base_.purpose != CIRCUIT_PURPOSE_S_INTRO) {
       
      continue;
    }

     
    intro_svc->listed_in_last_desc = 1;

     
    intro_desc = tor_malloc_zero(sizeof(rend_intro_point_t));
    intro_desc->extend_info = extend_info_dup(intro_svc->extend_info);
    if (intro_svc->intro_key)
      intro_desc->intro_key = crypto_pk_dup_key(intro_svc->intro_key);
    smartlist_add(d->intro_nodes, intro_desc);

    if (intro_svc->time_published == -1) {
       
      intro_svc->time_published = time(NULL);
    }
  }
}

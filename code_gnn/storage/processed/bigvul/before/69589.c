rend_add_service(smartlist_t *service_list, rend_service_t *service)
{
  int i;
  rend_service_port_config_t *p;

  tor_assert(service);

  smartlist_t *s_list = rend_get_service_list_mutable(service_list);
   
  if (BUG(!s_list)) {
    return -1;
  }

  service->intro_nodes = smartlist_new();
  service->expiring_nodes = smartlist_new();

  if (service->max_streams_per_circuit < 0) {
    log_warn(LD_CONFIG, "Hidden service (%s) configured with negative max "
                        "streams per circuit.",
             rend_service_escaped_dir(service));
    rend_service_free(service);
    return -1;
  }

  if (service->max_streams_close_circuit < 0 ||
      service->max_streams_close_circuit > 1) {
    log_warn(LD_CONFIG, "Hidden service (%s) configured with invalid "
                        "max streams handling.",
             rend_service_escaped_dir(service));
    rend_service_free(service);
    return -1;
  }

  if (service->auth_type != REND_NO_AUTH &&
      (!service->clients ||
       smartlist_len(service->clients) == 0)) {
    log_warn(LD_CONFIG, "Hidden service (%s) with client authorization but no "
                        "clients.",
             rend_service_escaped_dir(service));
    rend_service_free(service);
    return -1;
  }

  if (!service->ports || !smartlist_len(service->ports)) {
    log_warn(LD_CONFIG, "Hidden service (%s) with no ports configured.",
             rend_service_escaped_dir(service));
    rend_service_free(service);
    return -1;
  } else {
    int dupe = 0;
     
    tor_assert(s_list);
    if (!rend_service_is_ephemeral(service)) {
       
      SMARTLIST_FOREACH(s_list, rend_service_t*, ptr,
                        dupe = dupe ||
                               !strcmp(ptr->directory, service->directory));
      if (dupe) {
        log_warn(LD_REND, "Another hidden service is already configured for "
                 "directory %s.",
                 rend_service_escaped_dir(service));
        rend_service_free(service);
        return -1;
      }
    }
    log_debug(LD_REND,"Configuring service with directory %s",
              rend_service_escaped_dir(service));
    for (i = 0; i < smartlist_len(service->ports); ++i) {
      p = smartlist_get(service->ports, i);
      if (!(p->is_unix_addr)) {
        log_debug(LD_REND,
                  "Service maps port %d to %s",
                  p->virtual_port,
                  fmt_addrport(&p->real_addr, p->real_port));
      } else {
#ifdef HAVE_SYS_UN_H
        log_debug(LD_REND,
                  "Service maps port %d to socket at \"%s\"",
                  p->virtual_port, p->unix_addr);
#else
        log_warn(LD_BUG,
                 "Service maps port %d to an AF_UNIX socket, but we "
                 "have no AF_UNIX support on this platform.  This is "
                 "probably a bug.",
                 p->virtual_port);
        rend_service_free(service);
        return -1;
#endif  
      }
    }
     
    tor_assert(s_list);
    smartlist_add(s_list, service);
    return 0;
  }
   
}
rend_service_use_direct_connection(const or_options_t* options,
                                   const extend_info_t* ei)
{
   
  return (rend_service_allow_non_anonymous_connection(options) &&
          fascist_firewall_allows_address_addr(&ei->addr, ei->port,
                                               FIREWALL_OR_CONNECTION, 0, 0));
}

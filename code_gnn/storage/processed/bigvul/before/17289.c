eXosip_listen_addr (struct eXosip_t *excontext, int transport, const char *addr, int port, int family, int secure)
{
  int i = -1;

  if (excontext->eXtl_transport.enabled > 0) {
     
    OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "eXosip: already listening somewhere\n"));
    return OSIP_WRONG_STATE;
  }

  if (transport == IPPROTO_UDP && secure == 0)
    eXosip_transport_udp_init (excontext);
  else if (transport == IPPROTO_TCP && secure == 0)
    eXosip_transport_tcp_init (excontext);
#ifdef HAVE_OPENSSL_SSL_H
#if !(OPENSSL_VERSION_NUMBER < 0x00908000L)
  else if (transport == IPPROTO_UDP)
    eXosip_transport_dtls_init (excontext);
#endif
  else if (transport == IPPROTO_TCP)
    eXosip_transport_tls_init (excontext);
#endif
  else
    return OSIP_BADPARAMETER;

  if (excontext->eXtl_transport.tl_init != NULL)
    excontext->eXtl_transport.tl_init (excontext);

  excontext->eXtl_transport.proto_family = family;
  excontext->eXtl_transport.proto_port = port;
  if (addr != NULL)
    snprintf (excontext->eXtl_transport.proto_ifs, sizeof (excontext->eXtl_transport.proto_ifs), "%s", addr);

#ifdef  AF_INET6
  if (family == AF_INET6 && !addr)
    snprintf (excontext->eXtl_transport.proto_ifs, sizeof (excontext->eXtl_transport.proto_ifs), "::0");
#endif

  i = excontext->eXtl_transport.tl_open (excontext);

  if (i != 0) {
    if (excontext->eXtl_transport.tl_free != NULL)
      excontext->eXtl_transport.tl_free (excontext);
    return i;
  }

  if (transport == IPPROTO_UDP && secure == 0)
    snprintf (excontext->transport, sizeof (excontext->transport), "%s", "UDP");
  else if (transport == IPPROTO_TCP && secure == 0)
    snprintf (excontext->transport, sizeof (excontext->transport), "%s", "TCP");
  else if (transport == IPPROTO_UDP)
    snprintf (excontext->transport, sizeof (excontext->transport), "%s", "DTLS-UDP");
  else if (transport == IPPROTO_TCP)
    snprintf (excontext->transport, sizeof (excontext->transport), "%s", "TLS");

#ifndef OSIP_MONOTHREAD
  if (excontext->j_thread == NULL) {
    excontext->j_thread = (void *) osip_thread_create (20000, _eXosip_thread, excontext);
    if (excontext->j_thread == NULL) {
      OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "eXosip: Cannot start thread!\n"));
      return OSIP_UNDEFINED_ERROR;
    }
  }
#endif

  return OSIP_SUCCESS;
}

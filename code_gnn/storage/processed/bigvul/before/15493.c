persistent_available_p (const char *host, int port, bool ssl,
                        bool *host_lookup_failed)
{
   
  if (!pconn_active)
    return false;

   
  if (ssl != pconn.ssl)
    return false;

   
  if (port != pconn.port)
    return false;

   
  if (0 != strcasecmp (host, pconn.host))
    {
       

      bool found;
      ip_address ip;
      struct address_list *al;

      if (ssl)
         
        return false;

       

      if (!socket_ip_address (pconn.socket, &ip, ENDPOINT_PEER))
        {
           
          invalidate_persistent ();
          return false;
        }
      al = lookup_host (host, 0);
      if (!al)
        {
          *host_lookup_failed = true;
          return false;
        }

      found = address_list_contains (al, &ip);
      address_list_release (al);

      if (!found)
        return false;

       
    }

   

  if (!test_socket_open (pconn.socket))
    {
       
      invalidate_persistent ();
      return false;
    }

  return true;
}

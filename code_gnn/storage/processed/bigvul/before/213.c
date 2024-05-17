ftp_do_pasv (int csock, ip_address *addr, int *port)
{
  uerr_t err;

   
  if (!socket_ip_address (csock, addr, ENDPOINT_PEER))
    abort ();

   
  switch (addr->family)
    {
    case AF_INET:
      if (!opt.server_response)
        logputs (LOG_VERBOSE, "==> PASV ... ");
      err = ftp_pasv (csock, addr, port);
      break;
    case AF_INET6:
      if (!opt.server_response)
        logputs (LOG_VERBOSE, "==> EPSV ... ");
      err = ftp_epsv (csock, addr, port);

       
      if (err == FTPNOPASV)
        {
          if (!opt.server_response)
            logputs (LOG_VERBOSE, "==> LPSV ... ");
          err = ftp_lpsv (csock, addr, port);
        }
      break;
    default:
      abort ();
    }

  return err;
}

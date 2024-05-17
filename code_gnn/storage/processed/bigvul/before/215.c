ftp_do_port (int csock, int *local_sock)
{
  uerr_t err;
  ip_address cip;

  if (!socket_ip_address (csock, &cip, ENDPOINT_PEER))
    abort ();

   
  switch (cip.family)
    {
    case AF_INET:
      if (!opt.server_response)
        logputs (LOG_VERBOSE, "==> PORT ... ");
      err = ftp_port (csock, local_sock);
      break;
    case AF_INET6:
      if (!opt.server_response)
        logputs (LOG_VERBOSE, "==> EPRT ... ");
      err = ftp_eprt (csock, local_sock);

       
      if (err == FTPPORTERR)
        {
          if (!opt.server_response)
            logputs (LOG_VERBOSE, "==> LPRT ... ");
          err = ftp_lprt (csock, local_sock);
        }
      break;
    default:
      abort ();
    }
  return err;
}

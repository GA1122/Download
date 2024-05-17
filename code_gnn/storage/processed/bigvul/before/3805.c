_dbus_listen_tcp_socket (const char     *host,
                         const char     *port,
                         const char     *family,
                         DBusString     *retport,
                         int           **fds_p,
                         DBusError      *error)
{
  int nlisten_fd = 0, *listen_fd = NULL, res, i, port_num = -1;
  struct addrinfo hints;
  struct addrinfo *ai, *tmp;


  typedef union {
	struct sockaddr Address;
	struct sockaddr_in AddressIn;
	struct sockaddr_in6 AddressIn6;
  } mysockaddr_gen;

  *fds_p = NULL;
  _DBUS_ASSERT_ERROR_IS_CLEAR (error);

  _dbus_win_startup_winsock ();

  _DBUS_ZERO (hints);

  if (!family)
    hints.ai_family = AF_UNSPEC;
  else if (!strcmp(family, "ipv4"))
    hints.ai_family = AF_INET;
  else if (!strcmp(family, "ipv6"))
    hints.ai_family = AF_INET6;
  else
    {
      dbus_set_error (error,
                      DBUS_ERROR_INVALID_ARGS,
                      "Unknown address family %s", family);
      return -1;
    }

  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
  hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;
#else
  hints.ai_flags = AI_PASSIVE;
#endif

 redo_lookup_with_port:
  if ((res = getaddrinfo(host, port, &hints, &ai)) != 0 || !ai)
    {
      dbus_set_error (error,
                      _dbus_error_from_errno (res),
                      "Failed to lookup host/port: \"%s:%s\": %s (%d)",
                      host ? host : "*", port, _dbus_strerror(res), res);
      return -1;
    }

  tmp = ai;
  while (tmp)
    {
      int fd = -1, *newlisten_fd;
      if ((fd = socket (tmp->ai_family, SOCK_STREAM, 0)) == INVALID_SOCKET)
        {
          DBUS_SOCKET_SET_ERRNO ();
          dbus_set_error (error,
                          _dbus_error_from_errno (errno),
                         "Failed to open socket: %s",
                         _dbus_strerror_from_errno ());
          goto failed;
        }
      _DBUS_ASSERT_ERROR_IS_CLEAR(error);

      if (bind (fd, (struct sockaddr*) tmp->ai_addr, tmp->ai_addrlen) == SOCKET_ERROR)
        {
          DBUS_SOCKET_SET_ERRNO ();
          dbus_set_error (error, _dbus_error_from_errno (errno),
                          "Failed to bind socket \"%s:%s\": %s",
                          host ? host : "*", port, _dbus_strerror_from_errno ());
          closesocket (fd);
          goto failed;
    }

      if (listen (fd, 30  ) == SOCKET_ERROR)
        {
          DBUS_SOCKET_SET_ERRNO ();
          dbus_set_error (error, _dbus_error_from_errno (errno),
                          "Failed to listen on socket \"%s:%s\": %s",
                          host ? host : "*", port, _dbus_strerror_from_errno ());
          closesocket (fd);
          goto failed;
        }

      newlisten_fd = dbus_realloc(listen_fd, sizeof(int)*(nlisten_fd+1));
      if (!newlisten_fd)
        {
          closesocket (fd);
          dbus_set_error (error, DBUS_ERROR_NO_MEMORY,
                          "Failed to allocate file handle array");
          goto failed;
        }
      listen_fd = newlisten_fd;
      listen_fd[nlisten_fd] = fd;
      nlisten_fd++;

      if (!_dbus_string_get_length(retport))
        {
           
          if (!port || !strcmp(port, "0"))
            {
              mysockaddr_gen addr;
              socklen_t addrlen = sizeof(addr);
              char portbuf[10];

              if (getsockname(fd, &addr.Address, &addrlen) == SOCKET_ERROR)
                {
                  DBUS_SOCKET_SET_ERRNO ();
                  dbus_set_error (error, _dbus_error_from_errno (errno),
                                  "Failed to resolve port \"%s:%s\": %s",
                                  host ? host : "*", port, _dbus_strerror_from_errno());
                  goto failed;
                }
              snprintf( portbuf, sizeof( portbuf ) - 1, "%d", addr.AddressIn.sin_port );
              if (!_dbus_string_append(retport, portbuf))
                {
                  dbus_set_error (error, DBUS_ERROR_NO_MEMORY, NULL);
                  goto failed;
                }

               
              port = _dbus_string_get_const_data(retport);
              freeaddrinfo(ai);
              goto redo_lookup_with_port;
            }
          else
            {
              if (!_dbus_string_append(retport, port))
                {
                    dbus_set_error (error, DBUS_ERROR_NO_MEMORY, NULL);
                    goto failed;
                }
            }
        }
  
      tmp = tmp->ai_next;
    }
  freeaddrinfo(ai);
  ai = NULL;

  if (!nlisten_fd)
    {
      _dbus_win_set_errno (WSAEADDRINUSE);
      dbus_set_error (error, _dbus_error_from_errno (errno),
                      "Failed to bind socket \"%s:%s\": %s",
                      host ? host : "*", port, _dbus_strerror_from_errno ());
      return -1;
    }

  sscanf(_dbus_string_get_const_data(retport), "%d", &port_num);

  for (i = 0 ; i < nlisten_fd ; i++)
    {
      _dbus_fd_set_close_on_exec (listen_fd[i]);
      if (!_dbus_set_fd_nonblocking (listen_fd[i], error))
        {
          goto failed;
        }
    }

  *fds_p = listen_fd;

  return nlisten_fd;

 failed:
  if (ai)
    freeaddrinfo(ai);
  for (i = 0 ; i < nlisten_fd ; i++)
    closesocket (listen_fd[i]);
  dbus_free(listen_fd);
  return -1;
}

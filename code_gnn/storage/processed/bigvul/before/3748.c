_dbus_listen_unix_socket (const char     *path,
                          dbus_bool_t     abstract,
                          DBusError      *error)
{
  int listen_fd;
  struct sockaddr_un addr;
  size_t path_len;
  unsigned int reuseaddr;

  _DBUS_ASSERT_ERROR_IS_CLEAR (error);

  _dbus_verbose ("listening on unix socket %s abstract=%d\n",
                 path, abstract);

  if (!_dbus_open_unix_socket (&listen_fd, error))
    {
      _DBUS_ASSERT_ERROR_IS_SET(error);
      return -1;
    }
  _DBUS_ASSERT_ERROR_IS_CLEAR(error);

  _DBUS_ZERO (addr);
  addr.sun_family = AF_UNIX;
  path_len = strlen (path);

  if (abstract)
    {
#ifdef HAVE_ABSTRACT_SOCKETS
       
      addr.sun_path[0] = '\0';  
      path_len++;  

      if (path_len > _DBUS_MAX_SUN_PATH_LENGTH)
        {
          dbus_set_error (error, DBUS_ERROR_BAD_ADDRESS,
                      "Abstract socket name too long\n");
          _dbus_close (listen_fd, NULL);
          return -1;
	}

      strncpy (&addr.sun_path[1], path, path_len);
       
#else  
      dbus_set_error (error, DBUS_ERROR_NOT_SUPPORTED,
                      "Operating system does not support abstract socket namespace\n");
      _dbus_close (listen_fd, NULL);
      return -1;
#endif  
    }
  else
    {
       
      {
        struct stat sb;

        if (stat (path, &sb) == 0 &&
            S_ISSOCK (sb.st_mode))
          unlink (path);
      }

      if (path_len > _DBUS_MAX_SUN_PATH_LENGTH)
        {
          dbus_set_error (error, DBUS_ERROR_BAD_ADDRESS,
                      "Abstract socket name too long\n");
          _dbus_close (listen_fd, NULL);
          return -1;
	}

      strncpy (addr.sun_path, path, path_len);
    }

  reuseaddr = 1;
  if (setsockopt  (listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr))==-1)
    {
      _dbus_warn ("Failed to set socket option\"%s\": %s",
                  path, _dbus_strerror (errno));
    }

  if (bind (listen_fd, (struct sockaddr*) &addr, _DBUS_STRUCT_OFFSET (struct sockaddr_un, sun_path) + path_len) < 0)
    {
      dbus_set_error (error, _dbus_error_from_errno (errno),
                      "Failed to bind socket \"%s\": %s",
                      path, _dbus_strerror (errno));
      _dbus_close (listen_fd, NULL);
      return -1;
    }

  if (listen (listen_fd, 30  ) < 0)
    {
      dbus_set_error (error, _dbus_error_from_errno (errno),
                      "Failed to listen on socket \"%s\": %s",
                      path, _dbus_strerror (errno));
      _dbus_close (listen_fd, NULL);
      return -1;
    }

  if (!_dbus_set_local_creds (listen_fd, TRUE))
    {
      dbus_set_error (error, _dbus_error_from_errno (errno),
                      "Failed to enable LOCAL_CREDS on socket \"%s\": %s",
                      path, _dbus_strerror (errno));
      close (listen_fd);
      return -1;
    }

  if (!_dbus_set_fd_nonblocking (listen_fd, error))
    {
      _DBUS_ASSERT_ERROR_IS_SET (error);
      _dbus_close (listen_fd, NULL);
      return -1;
    }

   
  if (!abstract && chmod (path, 0777) < 0)
    _dbus_warn ("Could not set mode 0777 on socket %s\n",
                path);

  return listen_fd;
}

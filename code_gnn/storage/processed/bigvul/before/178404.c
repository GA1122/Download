 gboolean lxterminal_socket_initialize(LXTermWindow * lxtermwin, gint argc, gchar * * argv)
 {
      
  
       
    gchar * socket_path = g_strdup_printf("/tmp/.lxterminal-socket%s-%s", gdk_display_get_name(gdk_display_get_default()), g_get_user_name());
//     gchar * socket_path = g_strdup_printf("%s/.lxterminal-socket-%s", g_get_user_runtime_dir(), gdk_display_get_name(gdk_display_get_default()));
//     printf("%s\n", socket_path);
  
       
      int fd = socket(PF_UNIX, SOCK_STREAM, 0);
     {
         g_warning("Socket create failed: %s\n", g_strerror(errno));
         g_free(socket_path);
         return TRUE;
     }
 
      
     struct sockaddr_un sock_addr;
     memset(&sock_addr, 0, sizeof(sock_addr));
     sock_addr.sun_family = AF_UNIX;
     snprintf(sock_addr.sun_path, sizeof(sock_addr.sun_path), "%s", socket_path);
 
      
     if (connect(fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
     {
          
         unlink(socket_path);
         g_free(socket_path);
 
          
         if (bind(fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
         {
             g_warning("Bind on socket failed: %s\n", g_strerror(errno));
             close(fd);
             return TRUE;
         }
 
          
         if (listen(fd, 5) < 0)
         {
             g_warning("Listen on socket failed: %s\n", g_strerror(errno));
             close(fd);
             return TRUE;
         }
 
          
         GIOChannel * gio = g_io_channel_unix_new(fd);
         if (gio == NULL)
         {
             g_warning("Cannot create GIOChannel\n");
             close(fd);
             return TRUE;
         }
 
          
         g_io_channel_set_encoding(gio, NULL, NULL);
         g_io_channel_set_buffered(gio, FALSE);
         g_io_channel_set_close_on_unref(gio, TRUE);
 
          
         if ( ! g_io_add_watch(gio, G_IO_IN | G_IO_HUP, (GIOFunc) lxterminal_socket_accept_client, lxtermwin))
         {
             g_warning("Cannot add watch on GIOChannel\n");
             close(fd);
             g_io_channel_unref(gio);
             return TRUE;
         }
 
          
         g_io_channel_set_close_on_unref(gio, TRUE);
         g_io_channel_unref(gio);
         return TRUE;
     }
     else
     {
         g_free(socket_path);
 
          
         GIOChannel * gio = g_io_channel_unix_new(fd);
         g_io_channel_set_encoding(gio, NULL, NULL);
 
          
 	gchar * cur_dir = g_get_current_dir();
         g_io_channel_write_chars(gio, cur_dir, -1, NULL, NULL);
 	 
 	g_io_channel_write_chars(gio, "", 1, NULL, NULL);
 	g_free(cur_dir);
 
          
 	gint i;
 	for (i = 0; i < argc; i ++)
 	{
             g_io_channel_write_chars(gio, argv[i], -1, NULL, NULL);
 	    g_io_channel_write_chars(gio, "", 1, NULL, NULL);
 	}
 
         g_io_channel_flush(gio, NULL);
         g_io_channel_unref(gio);
         return FALSE;
     }
 }
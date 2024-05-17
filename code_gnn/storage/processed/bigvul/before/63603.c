setup_listener (GsmXsmpServer *server)
{
        char   error[256];
        mode_t saved_umask;
        char  *network_id_list;
        int    i;
        int    res;

         
        IceSetErrorHandler (ice_error_handler);
        IceSetIOErrorHandler (ice_io_error_handler);
        SmsSetErrorHandler (sms_error_handler);

         
        res = SmsInitialize (PACKAGE,
                             VERSION,
                             (SmsNewClientProc)accept_xsmp_connection,
                             server,
                             NULL,
                             sizeof (error),
                             error);
        if (! res) {
                gsm_util_init_error (TRUE, "Could not initialize libSM: %s", error);
        }

#ifdef HAVE_X11_XTRANS_XTRANS_H
         
        _IceTransNoListen ("tcp");
#endif

         
        saved_umask = umask (0);
        umask (saved_umask);
        res = IceListenForConnections (&server->priv->num_xsmp_sockets,
                                       &server->priv->xsmp_sockets,
                                       sizeof (error),
                                       error);
        if (! res) {
                gsm_util_init_error (TRUE, _("Could not create ICE listening socket: %s"), error);
        }

        umask (saved_umask);

         
        for (i = server->priv->num_local_xsmp_sockets = 0; i < server->priv->num_xsmp_sockets; i++) {
                char *id = IceGetListenConnectionString (server->priv->xsmp_sockets[i]);

                if (!strncmp (id, "local/", sizeof ("local/") - 1) ||
                    !strncmp (id, "unix/", sizeof ("unix/") - 1)) {
                        if (i > server->priv->num_local_xsmp_sockets) {
                                IceListenObj tmp;
                                tmp = server->priv->xsmp_sockets[i];
                                server->priv->xsmp_sockets[i] = server->priv->xsmp_sockets[server->priv->num_local_xsmp_sockets];
                                server->priv->xsmp_sockets[server->priv->num_local_xsmp_sockets] = tmp;
                        }
                        server->priv->num_local_xsmp_sockets++;
                }
                free (id);
        }

        if (server->priv->num_local_xsmp_sockets == 0) {
                gsm_util_init_error (TRUE, "IceListenForConnections did not return a local listener!");
        }

#ifdef HAVE_X11_XTRANS_XTRANS_H
        if (server->priv->num_local_xsmp_sockets != server->priv->num_xsmp_sockets) {
                 
                network_id_list = IceComposeNetworkIdList (server->priv->num_xsmp_sockets - server->priv->num_local_xsmp_sockets,
                                                           server->priv->xsmp_sockets + server->priv->num_local_xsmp_sockets);
                g_warning ("IceListenForConnections returned %d non-local listeners: %s",
                           server->priv->num_xsmp_sockets - server->priv->num_local_xsmp_sockets,
                           network_id_list);
                free (network_id_list);
        }
#endif

         
        if (!update_iceauthority (server, TRUE)) {
                 
                gsm_util_init_error (TRUE,
                                     "Could not update ICEauthority file %s",
                                     IceAuthFileName ());
        }

        network_id_list = IceComposeNetworkIdList (server->priv->num_local_xsmp_sockets,
                                                   server->priv->xsmp_sockets);

        gsm_util_setenv ("SESSION_MANAGER", network_id_list);
        g_debug ("GsmXsmpServer: SESSION_MANAGER=%s\n", network_id_list);
        free (network_id_list);
}

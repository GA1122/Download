network_connect_with_fork (struct t_hook *hook_connect)
{
    int child_pipe[2];
#ifndef __CYGWIN__
    pid_t pid;
#endif
    
#ifdef HAVE_GNUTLS
     
    if (HOOK_CONNECT(hook_connect, gnutls_sess))
    {
        if (gnutls_init (HOOK_CONNECT(hook_connect, gnutls_sess), GNUTLS_CLIENT) != 0)
        {
            (void) (HOOK_CONNECT(hook_connect, callback))
                (hook_connect->callback_data,
                 '0' + WEECHAT_HOOK_CONNECT_GNUTLS_INIT_ERROR,
                 0, NULL, NULL);
            unhook (hook_connect);
            return;
        }
        gnutls_set_default_priority (*HOOK_CONNECT(hook_connect, gnutls_sess));
        gnutls_certificate_type_set_priority (*HOOK_CONNECT(hook_connect, gnutls_sess),
                                              gnutls_cert_type_prio);
        gnutls_protocol_set_priority (*HOOK_CONNECT(hook_connect, gnutls_sess),
                                      gnutls_prot_prio);
        gnutls_credentials_set (*HOOK_CONNECT(hook_connect, gnutls_sess),
                                GNUTLS_CRD_CERTIFICATE,
                                gnutls_xcred);
        gnutls_transport_set_ptr (*HOOK_CONNECT(hook_connect, gnutls_sess),
                                  (gnutls_transport_ptr) ((unsigned long) HOOK_CONNECT(hook_connect, sock)));
    }
#endif
    
     
    if (pipe (child_pipe) < 0)
    {
        (void) (HOOK_CONNECT(hook_connect, callback))
            (hook_connect->callback_data,
             '0' + WEECHAT_HOOK_CONNECT_MEMORY_ERROR,
             0, NULL, NULL);
        unhook (hook_connect);
        return;
    }
    HOOK_CONNECT(hook_connect, child_read) = child_pipe[0];
    HOOK_CONNECT(hook_connect, child_write) = child_pipe[1];
    
#ifdef __CYGWIN__
     
    network_connect_child (hook_connect);
    network_connect_child_read_cb (hook_connect, 0);
#else
    switch (pid = fork ())
    {
         
        case -1:
            (void) (HOOK_CONNECT(hook_connect, callback))
                (hook_connect->callback_data,
                 '0' + WEECHAT_HOOK_CONNECT_MEMORY_ERROR,
                 0, NULL, NULL);
            unhook (hook_connect);
            return;
         
        case 0:
            setuid (getuid ());
            close (HOOK_CONNECT(hook_connect, child_read));
            network_connect_child (hook_connect);
            _exit (EXIT_SUCCESS);
    }
     
    HOOK_CONNECT(hook_connect, child_pid) = pid;
    close (HOOK_CONNECT(hook_connect, child_write));
    HOOK_CONNECT(hook_connect, child_write) = -1;
    HOOK_CONNECT(hook_connect, hook_fd) = hook_fd (hook_connect->plugin,
                                                   HOOK_CONNECT(hook_connect, child_read),
                                                   1, 0, 0,
                                                   &network_connect_child_read_cb,
                                                   hook_connect);
#endif
}

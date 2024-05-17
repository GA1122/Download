network_connect_child_read_cb (void *arg_hook_connect, int fd)
{
    struct t_hook *hook_connect;
    char buffer[1], buf_size_ip[6], *ip_address, *error;
    int num_read;
    long size_ip;
#ifdef HAVE_GNUTLS
    int rc, direction;
#endif

     
    (void) fd;
    
    hook_connect = (struct t_hook *)arg_hook_connect;
    
    ip_address = NULL;
    
    num_read = read (HOOK_CONNECT(hook_connect, child_read),
                     buffer, sizeof (buffer));
    if (num_read > 0)
    {
        if (buffer[0] - '0' == WEECHAT_HOOK_CONNECT_OK)
        {
            buf_size_ip[5] = '\0';
            num_read = read (HOOK_CONNECT(hook_connect, child_read),
                             buf_size_ip, 5);
            if (num_read == 5)
            {
                error = NULL;
                size_ip = strtol (buf_size_ip, &error, 10);
                if (error && !error[0])
                {
                    ip_address = malloc (size_ip + 1);
                    if (ip_address)
                    {
                        num_read = read (HOOK_CONNECT(hook_connect, child_read),
                                         ip_address, size_ip);
                        if (num_read == size_ip)
                            ip_address[size_ip] = '\0';
                        else
                        {
                            free (ip_address);
                            ip_address = NULL;
                        }
                    }
                }
            }
#ifdef HAVE_GNUTLS
            if (HOOK_CONNECT(hook_connect, gnutls_sess))
            {
                 
                HOOK_CONNECT(hook_connect, handshake_fd_flags) =
                    fcntl (HOOK_CONNECT(hook_connect, sock), F_GETFL);
                fcntl (HOOK_CONNECT(hook_connect, sock), F_SETFL,
                       HOOK_CONNECT(hook_connect, handshake_fd_flags) | O_NONBLOCK);
                gnutls_transport_set_ptr (*HOOK_CONNECT(hook_connect, gnutls_sess),
                                          (gnutls_transport_ptr) ((unsigned long) HOOK_CONNECT(hook_connect, sock)));
                if (HOOK_CONNECT (hook_connect, gnutls_dhkey_size) > 0)
                {
                    gnutls_dh_set_prime_bits (*HOOK_CONNECT(hook_connect, gnutls_sess),
                                              (unsigned int) HOOK_CONNECT(hook_connect, gnutls_dhkey_size));
                }
                rc = gnutls_handshake (*HOOK_CONNECT(hook_connect, gnutls_sess));
                if ((rc == GNUTLS_E_AGAIN) || (rc == GNUTLS_E_INTERRUPTED))
                {
                     
                    unhook (HOOK_CONNECT(hook_connect, hook_fd));
                    HOOK_CONNECT(hook_connect, hook_fd) = NULL;
                    direction = gnutls_record_get_direction (*HOOK_CONNECT(hook_connect, gnutls_sess));
                    HOOK_CONNECT(hook_connect, handshake_ip_address) = ip_address;
                    HOOK_CONNECT(hook_connect, handshake_hook_fd) =
                        hook_fd (hook_connect->plugin,
                                 HOOK_CONNECT(hook_connect, sock),
                                 (!direction ? 1 : 0), (direction  ? 1 : 0), 0,
                                 &network_connect_gnutls_handshake_fd_cb,
                                 hook_connect);
                    HOOK_CONNECT(hook_connect, handshake_hook_timer) =
                        hook_timer (hook_connect->plugin,
                                    CONFIG_INTEGER(config_network_gnutls_handshake_timeout) * 1000,
                                    0, 1,
                                    &network_connect_gnutls_handshake_timer_cb,
                                    hook_connect);
                    return WEECHAT_RC_OK;
                }
                else if (rc != GNUTLS_E_SUCCESS)
                {
                    (void) (HOOK_CONNECT(hook_connect, callback))
                        (hook_connect->callback_data,
                         WEECHAT_HOOK_CONNECT_GNUTLS_HANDSHAKE_ERROR,
                         rc,
                         gnutls_strerror (rc),
                         ip_address);
                    unhook (hook_connect);
                    if (ip_address)
                        free (ip_address);
                    return WEECHAT_RC_OK;
                }
                fcntl (HOOK_CONNECT(hook_connect, sock), F_SETFL,
                       HOOK_CONNECT(hook_connect, handshake_fd_flags));
            }
#endif
        }
        (void) (HOOK_CONNECT(hook_connect, callback))
            (hook_connect->callback_data, buffer[0] - '0', 0, NULL, ip_address);
        unhook (hook_connect);
    }
    
    if (ip_address)
        free (ip_address);
    
    return WEECHAT_RC_OK;
}

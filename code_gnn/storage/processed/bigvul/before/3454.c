network_connect_gnutls_handshake_fd_cb (void *arg_hook_connect, int fd)
{
    struct t_hook *hook_connect;
    int rc, direction, flags;
    
     
    (void) fd;
    
    hook_connect = (struct t_hook *)arg_hook_connect;
    
    rc = gnutls_handshake (*HOOK_CONNECT(hook_connect, gnutls_sess));
    
    if ((rc == GNUTLS_E_AGAIN) || (rc == GNUTLS_E_INTERRUPTED))
    {
        direction = gnutls_record_get_direction (*HOOK_CONNECT(hook_connect, gnutls_sess));
        flags = HOOK_FD(HOOK_CONNECT(hook_connect, handshake_hook_fd), flags);
        if ((((flags & HOOK_FD_FLAG_READ) == HOOK_FD_FLAG_READ)
             && (direction != 0))
            || (((flags & HOOK_FD_FLAG_WRITE) == HOOK_FD_FLAG_WRITE)
                && (direction != 1)))
        {
            HOOK_FD(HOOK_CONNECT(hook_connect, handshake_hook_fd), flags) =
                (direction) ? HOOK_FD_FLAG_WRITE: HOOK_FD_FLAG_READ;
        }
    }
    else if (rc != GNUTLS_E_SUCCESS)
    {
        (void) (HOOK_CONNECT(hook_connect, callback))
            (hook_connect->callback_data,
             WEECHAT_HOOK_CONNECT_GNUTLS_HANDSHAKE_ERROR,
             rc,
             gnutls_strerror (rc),
             HOOK_CONNECT(hook_connect, handshake_ip_address));
        unhook (hook_connect);
    }
    else
    {
        fcntl (HOOK_CONNECT(hook_connect, sock), F_SETFL,
               HOOK_CONNECT(hook_connect, handshake_fd_flags));
        unhook (HOOK_CONNECT(hook_connect, handshake_hook_fd));
        (void) (HOOK_CONNECT(hook_connect, callback))
                (hook_connect->callback_data, WEECHAT_HOOK_CONNECT_OK, 0, NULL,
                 HOOK_CONNECT(hook_connect, handshake_ip_address));
        unhook (hook_connect);
    }
    
    return WEECHAT_RC_OK;
}

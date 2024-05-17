static const char *connect_to_host(Ssh ssh, const char *host, int port,
				   char **realhost, int nodelay, int keepalive)
{
    static const struct plug_function_table fn_table = {
	ssh_socket_log,
	ssh_closing,
	ssh_receive,
	ssh_sent,
	NULL
    };

    SockAddr addr;
    const char *err;
    char *loghost;
    int addressfamily, sshprot;

    ssh_hostport_setup(host, port, ssh->conf,
                       &ssh->savedhost, &ssh->savedport, &loghost);

    ssh->fn = &fn_table;                

     
    ssh->connshare = NULL;
    ssh->attempting_connshare = TRUE;   
    ssh->s = ssh_connection_sharing_init(ssh->savedhost, ssh->savedport,
                                         ssh->conf, ssh, &ssh->connshare);
    ssh->attempting_connshare = FALSE;
    if (ssh->s != NULL) {
         
        ssh->bare_connection = TRUE;
        ssh->do_ssh_init = do_ssh_connection_init;
        ssh->fullhostname = NULL;
        *realhost = dupstr(host);       
    } else {
         
        ssh->do_ssh_init = do_ssh_init;

         
        addressfamily = conf_get_int(ssh->conf, CONF_addressfamily);
        addr = name_lookup(host, port, realhost, ssh->conf, addressfamily,
                           ssh->frontend, "SSH connection");
        if ((err = sk_addr_error(addr)) != NULL) {
            sk_addr_free(addr);
            return err;
        }
        ssh->fullhostname = dupstr(*realhost);    

        ssh->s = new_connection(addr, *realhost, port,
                                0, 1, nodelay, keepalive,
                                (Plug) ssh, ssh->conf);
        if ((err = sk_socket_error(ssh->s)) != NULL) {
            ssh->s = NULL;
            notify_remote_exit(ssh->frontend);
            return err;
        }
    }

     
    sshprot = conf_get_int(ssh->conf, CONF_sshprot);
    assert(sshprot == 0 || sshprot == 3);
    if (sshprot == 0)
	 
	ssh->version = 1;
    if (sshprot == 3 && !ssh->bare_connection) {
	 
	ssh->version = 2;
	ssh_send_verstring(ssh, "SSH-", NULL);
    }

     
    if (*loghost) {
	sfree(*realhost);
	*realhost = dupstr(loghost);
    }

    return NULL;
}

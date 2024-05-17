static int do_ssh_connection_init(Ssh ssh, unsigned char c)
{
     
    static const char protoname[] =
        "SSHCONNECTION@putty.projects.tartarus.org-";

    struct do_ssh_connection_init_state {
	int crLine;
	int vslen;
	char version[10];
	char *vstring;
	int vstrsize;
	int i;
    };
    crState(do_ssh_connection_init_state);
    
    crBeginState;

     
    for (;;) {
        for (s->i = 0; protoname[s->i]; s->i++) {
            if ((char)c != protoname[s->i]) goto no;
            crReturn(1);
        }
	break;
      no:
	while (c != '\012')
	    crReturn(1);
	crReturn(1);
    }

    s->vstrsize = sizeof(protoname) + 16;
    s->vstring = snewn(s->vstrsize, char);
    strcpy(s->vstring, protoname);
    s->vslen = strlen(protoname);
    s->i = 0;
    while (1) {
	if (s->vslen >= s->vstrsize - 1) {
	    s->vstrsize += 16;
	    s->vstring = sresize(s->vstring, s->vstrsize, char);
	}
	s->vstring[s->vslen++] = c;
	if (s->i >= 0) {
	    if (c == '-') {
		s->version[s->i] = '\0';
		s->i = -1;
	    } else if (s->i < sizeof(s->version) - 1)
		s->version[s->i++] = c;
	} else if (c == '\012')
	    break;
	crReturn(1);		        
    }

    ssh->agentfwd_enabled = FALSE;
    ssh->rdpkt2_bare_state.incoming_sequence = 0;

    s->vstring[s->vslen] = 0;
    s->vstring[strcspn(s->vstring, "\015\012")] = '\0'; 
    logeventf(ssh, "Server version: %s", s->vstring);
    ssh_detect_bugs(ssh, s->vstring);

     
    if (ssh_versioncmp(s->version, "2.0") < 0) {
	bombout(("Server announces compatibility with SSH-1 in bare ssh-connection protocol"));
        crStop(0);
    }
    if (conf_get_int(ssh->conf, CONF_sshprot) == 0) {
	bombout(("Bare ssh-connection protocol cannot be run in SSH-1-only mode"));
	crStop(0);
    }

    ssh->version = 2;

    logeventf(ssh, "Using bare ssh-connection protocol");

     
    ssh_send_verstring(ssh, protoname, s->version);

     
    ssh->protocol = ssh2_bare_connection_protocol;
    ssh2_bare_connection_protocol_setup(ssh);
    ssh->s_rdpkt = ssh2_bare_connection_rdpkt;

    update_specials_menu(ssh->frontend);
    ssh->state = SSH_STATE_BEFORE_SIZE;
    ssh->pinger = pinger_new(ssh->conf, &ssh_backend, ssh);

     
    do_ssh2_authconn(ssh, NULL, 0, NULL);

    sfree(s->vstring);

    crFinish(0);
}

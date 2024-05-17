static void ssh_gotdata(Ssh ssh, const unsigned char *data, int datalen)
{
     
    if (ssh->logctx)
	log_packet(ssh->logctx, PKT_INCOMING, -1, NULL, data, datalen,
		   0, NULL, NULL, 0, NULL);

    crBegin(ssh->ssh_gotdata_crstate);

     
    while (1) {
	int ret;		        
	if (datalen == 0)
	    crReturnV;		        
	ret = ssh->do_ssh_init(ssh, *data);
	data++;
	datalen--;
	if (ret == 0)
	    break;
    }

     

    while (1) {
	while (bufchain_size(&ssh->queued_incoming_data) > 0 || datalen > 0) {
	    if (ssh->frozen) {
		ssh_queue_incoming_data(ssh, &data, &datalen);
		 
		break;
	    } else if (bufchain_size(&ssh->queued_incoming_data) > 0) {
		 
		ssh_process_queued_incoming_data(ssh);
	    } else {
		 
		ssh_process_incoming_data(ssh, &data, &datalen);
	    }
	     
	    if (ssh->state == SSH_STATE_CLOSED)
		return;
	}
	 
	crReturnV;
    }
    crFinishV;
}

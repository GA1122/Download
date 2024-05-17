SSH_PACKET_CALLBACK(ssh_packet_ignore_callback){
    (void)session;  
	(void)user;
	(void)type;
	(void)packet;
	SSH_LOG(SSH_LOG_PROTOCOL,"Received %s packet",type==SSH2_MSG_IGNORE ? "SSH_MSG_IGNORE" : "SSH_MSG_DEBUG");
	 
	return SSH_PACKET_USED;
}

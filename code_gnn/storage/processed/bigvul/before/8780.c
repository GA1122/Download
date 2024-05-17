static int udhcp_raw_socket(int ifindex)
{
	int fd;
	struct sockaddr_ll sock;

	log2("opening raw socket on ifindex %d", ifindex);

	fd = xsocket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));
	 
	log2("got raw socket fd");

	memset(&sock, 0, sizeof(sock));  
	sock.sll_family = AF_PACKET;
	sock.sll_protocol = htons(ETH_P_IP);
	sock.sll_ifindex = ifindex;
	 
	 
	 
	 
	xbind(fd, (struct sockaddr *) &sock, sizeof(sock));

#if 0  
	if (CLIENT_PORT == 68) {
		 
		 
		static const struct sock_filter filter_instr[] = {
			 
			BPF_STMT(BPF_LD|BPF_B|BPF_ABS, 9),
			 
			BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K, IPPROTO_UDP, 0, 6),
			 
			BPF_STMT(BPF_LD|BPF_H|BPF_ABS, 6),
			 
			BPF_JUMP(BPF_JMP|BPF_JSET|BPF_K, 0x1fff, 4, 0),
			 
			BPF_STMT(BPF_LDX|BPF_B|BPF_MSH, 0),
			 
			BPF_STMT(BPF_LD|BPF_H|BPF_IND, 2),
			 
			BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K, 68, 0, 1),
			 
			 
			BPF_STMT(BPF_RET|BPF_K, 0x7fffffff),
			 
			BPF_STMT(BPF_RET|BPF_K, 0),
		};
		static const struct sock_fprog filter_prog = {
			.len = sizeof(filter_instr) / sizeof(filter_instr[0]),
			 
			.filter = (struct sock_filter *) filter_instr,
		};
		 
		if (setsockopt(fd, SOL_SOCKET, SO_ATTACH_FILTER, &filter_prog,
				sizeof(filter_prog)) >= 0)
			log1("attached filter to raw socket fd");  
	}
#endif

	if (setsockopt_1(fd, SOL_PACKET, PACKET_AUXDATA) != 0) {
		if (errno != ENOPROTOOPT)
			log1("can't set PACKET_AUXDATA on raw socket");
	}

	log1("created raw socket");

	return fd;
}

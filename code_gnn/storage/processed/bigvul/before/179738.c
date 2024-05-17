 static int irda_create(struct net *net, struct socket *sock, int protocol,
 		       int kern)
 {
  	struct sock *sk;
  	struct irda_sock *self;
  
// 	if (protocol < 0 || protocol > SK_PROTOCOL_MAX)
// 		return -EINVAL;
// 
  	if (net != &init_net)
  		return -EAFNOSUPPORT;
  
 	 
 	switch (sock->type) {
 	case SOCK_STREAM:      
 	case SOCK_SEQPACKET:   
 	case SOCK_DGRAM:       
 		break;
 	default:
 		return -ESOCKTNOSUPPORT;
 	}
 
 	 
 	sk = sk_alloc(net, PF_IRDA, GFP_KERNEL, &irda_proto, kern);
 	if (sk == NULL)
 		return -ENOMEM;
 
 	self = irda_sk(sk);
 	pr_debug("%s() : self is %p\n", __func__, self);
 
 	init_waitqueue_head(&self->query_wait);
 
 	switch (sock->type) {
 	case SOCK_STREAM:
 		sock->ops = &irda_stream_ops;
 		self->max_sdu_size_rx = TTP_SAR_DISABLE;
 		break;
 	case SOCK_SEQPACKET:
 		sock->ops = &irda_seqpacket_ops;
 		self->max_sdu_size_rx = TTP_SAR_UNBOUND;
 		break;
 	case SOCK_DGRAM:
 		switch (protocol) {
 #ifdef CONFIG_IRDA_ULTRA
 		case IRDAPROTO_ULTRA:
 			sock->ops = &irda_ultra_ops;
 			 
 			self->max_data_size = ULTRA_MAX_DATA - LMP_PID_HEADER;
 			self->max_header_size = IRDA_MAX_HEADER + LMP_PID_HEADER;
 			break;
 #endif  
 		case IRDAPROTO_UNITDATA:
 			sock->ops = &irda_dgram_ops;
 			 
 			self->max_sdu_size_rx = TTP_SAR_UNBOUND;
 			break;
 		default:
 			sk_free(sk);
 			return -ESOCKTNOSUPPORT;
 		}
 		break;
 	default:
 		sk_free(sk);
 		return -ESOCKTNOSUPPORT;
 	}
 
 	 
 	sock_init_data(sock, sk);	 
 	sk->sk_family = PF_IRDA;
 	sk->sk_protocol = protocol;
 
 	 
 	self->ckey = irlmp_register_client(0, NULL, NULL, NULL);
 	self->mask.word = 0xffff;
 	self->rx_flow = self->tx_flow = FLOW_START;
 	self->nslots = DISCOVERY_DEFAULT_SLOTS;
 	self->daddr = DEV_ADDR_ANY;	 
 	self->saddr = 0x0;		 
 	return 0;
 }
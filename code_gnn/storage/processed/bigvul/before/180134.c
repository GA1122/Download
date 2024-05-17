 static int handle_packet(unsigned char *data, int data_len) {
 	struct mt_mactelnet_hdr pkthdr;
 
 	 
 	if (data_len < MT_HEADER_LEN){
 		return -1;
 	}
 	parse_packet(data, &pkthdr);
 
 	 
 	if (pkthdr.seskey != sessionkey) {
 		return -1;
 	}
 
 	 
 	if (pkthdr.ptype == MT_PTYPE_DATA) {
 		struct mt_packet odata;
 		struct mt_mactelnet_control_hdr cpkt;
 		int success = 0;
 
 		 
 		init_packet(&odata, MT_PTYPE_ACK, srcmac, dstmac, sessionkey, pkthdr.counter + (data_len - MT_HEADER_LEN));
 		send_udp(&odata, 0);
 
 		 
 		if (pkthdr.counter > incounter || (incounter - pkthdr.counter) > 65535) {
 			incounter = pkthdr.counter;
 		} else {
 			 
 			return -1;
 		}
 
 		 
 		success = parse_control_packet(data + MT_HEADER_LEN, data_len - MT_HEADER_LEN, &cpkt);
 
 		while (success) {
  
  			 
  			if (cpkt.cptype == MT_CPTYPE_PASSSALT) {
				memcpy(pass_salt, cpkt.data, cpkt.length);
// 				 
// 				if (cpkt.length != 16) {
// 					fprintf(stderr, _("Invalid salt length: %d (instead of 16) received from server %s\n"), cpkt.length, ether_ntoa((struct ether_addr *)dstmac));
// 				}
// 				memcpy(pass_salt, cpkt.data, 16);
  				send_auth(username, password);
  			}
  
 			 
 			else if (cpkt.cptype == MT_CPTYPE_PLAINDATA) {
 				fwrite((const void *)cpkt.data, 1, cpkt.length, stdout);
 			}
 
 			 
 			else if (cpkt.cptype == MT_CPTYPE_END_AUTH) {
 
 				 
 				terminal_mode = 1;
 
 				if (is_a_tty) {
 					 
 					raw_term();
 
 					setvbuf(stdin,  (char*)NULL, _IONBF, 0);
 
 					 
 					signal(SIGWINCH, sig_winch);
 				}
 			}
 
 			 
 			success = parse_control_packet(NULL, 0, &cpkt);
 		}
 	}
 	else if (pkthdr.ptype == MT_PTYPE_ACK) {
 		 
 	}
 
 	 
 	else if (pkthdr.ptype == MT_PTYPE_END) {
 		struct mt_packet odata;
 
 		 
 		init_packet(&odata, MT_PTYPE_END, srcmac, dstmac, pkthdr.seskey, 0);
 		send_udp(&odata, 0);
 
 		if (!quiet_mode) {
 			fprintf(stderr, _("Connection closed.\n"));
 		}
 
 		 
 		running = 0;
 	} else {
 		fprintf(stderr, _("Unhandeled packet type: %d received from server %s\n"), pkthdr.ptype, ether_ntoa((struct ether_addr *)dstmac));
 		return -1;
 	}
 
 	return pkthdr.ptype;
 }
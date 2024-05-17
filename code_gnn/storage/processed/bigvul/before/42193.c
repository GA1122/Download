static void *service_connection(void *arg_void)
{
	struct service_thread_param *arg =
		(struct service_thread_param *)arg_void;

	while (!arg->tcp->is_closed) {
		struct usb_conn_t *usb = NULL;
		struct http_message_t *server_msg = NULL;
		struct http_message_t *client_msg = NULL;

		client_msg = http_message_new();
		if (client_msg == NULL) {
			ERR("Failed to create client message");
			break;
		}
		NOTE("M %p: Client msg starting", client_msg);

		while (!client_msg->is_completed) {
			struct http_packet_t *pkt;
			pkt = tcp_packet_get(arg->tcp, client_msg);
			if (pkt == NULL) {
				if (arg->tcp->is_closed) {
					NOTE("M %p: Client closed connection\n", client_msg);
					goto cleanup_subconn;
				}
				ERR("M %p: Got null packet from tcp", client_msg);
				goto cleanup_subconn;
			}
			if (usb == NULL && arg->usb_sock != NULL) {
				usb = usb_conn_acquire(arg->usb_sock, 1);
				if (usb == NULL) {
					ERR("M %p: Failed to acquire usb interface", client_msg);
					packet_free(pkt);
					goto cleanup_subconn;
				}
				NOTE("M %p: Interface #%d: acquired usb conn",
				     client_msg,
				     usb->interface_index);
			}

			NOTE("M %p P %p: Pkt from tcp (buffer size: %d)\n===\n%.*s\n===", client_msg, pkt, pkt->filled_size, (int)pkt->filled_size, pkt->buffer);
			if (arg->usb_sock != NULL) {
				usb_conn_packet_send(usb, pkt);
				NOTE("M %p P %p: Interface #%d: Client pkt done",
				     client_msg, pkt, usb->interface_index);
			}
			packet_free(pkt);
		}
		if (usb != NULL)
			NOTE("M %p: Interface #%d: Client msg completed\n", client_msg,
			     usb->interface_index);
		else
			NOTE("M %p: Client msg completed\n", client_msg);
		message_free(client_msg);
		client_msg = NULL;


		server_msg = http_message_new();
		if (server_msg == NULL) {
			ERR("Failed to create server message");
			goto cleanup_subconn;
		}
		if (usb != NULL)
			NOTE("M %p: Interface #%d: Server msg starting", server_msg,
			     usb->interface_index);
		else
			NOTE("M %p: Server msg starting", server_msg);
		while (!server_msg->is_completed) {
			struct http_packet_t *pkt;
			if (arg->usb_sock != NULL) {
				pkt = usb_conn_packet_get(usb, server_msg);
				if (pkt == NULL)
					break;
			} else {
				pkt = packet_new(server_msg);
				snprintf((char*)(pkt->buffer),
					 pkt->buffer_capacity - 1,
					 "HTTP/1.1 200 OK\r\nContent-Type: text/html; name=ippusbxd.html; charset=UTF-8\r\n\r\n<html><h2>ippusbxd</h2><p>Debug/development mode without connection to IPP-over-USB printer</p></html>\r\n");
				pkt->filled_size = 183;
				server_msg->is_completed = 1;
				arg->tcp->is_closed = 1;
			}

			NOTE("M %p P %p: Pkt from usb (buffer size: %d)\n===\n%.*s\n===",
			     server_msg, pkt, pkt->filled_size,
			     (int)pkt->filled_size, pkt->buffer);
			tcp_packet_send(arg->tcp, pkt);
			if (usb != NULL)
				NOTE("M %p P %p: Interface #%d: Server pkt done",
				     server_msg, pkt, usb->interface_index);
			else
				NOTE("M %p P %p: Server pkt done",
				     server_msg, pkt);
			packet_free(pkt);
		}
		if (usb != NULL)
			NOTE("M %p: Interface #%d: Server msg completed\n", server_msg,
			     usb->interface_index);
		else
			NOTE("M %p: Server msg completed\n", server_msg);

cleanup_subconn:
		if (client_msg != NULL)
			message_free(client_msg);
		if (server_msg != NULL)
			message_free(server_msg);
		if (usb != NULL)
			usb_conn_release(usb);
	}



	tcp_conn_close(arg->tcp);
	free(arg);
	return NULL;
}
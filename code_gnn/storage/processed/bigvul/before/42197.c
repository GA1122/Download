struct http_packet_t *tcp_packet_get(struct tcp_conn_t *tcp,
                                     struct http_message_t *msg)
{
	struct http_packet_t *pkt = packet_new(msg);
	if (pkt == NULL) {
		ERR("failed to create packet for incoming tcp message");
		goto error;
	}

	size_t want_size = packet_pending_bytes(pkt);
	if (want_size == 0) {
		NOTE("TCP: Got %lu from spare buffer", pkt->filled_size);
		return pkt;
	}

	while (want_size != 0 && !msg->is_completed) {
		NOTE("TCP: Getting %d bytes", want_size);
		uint8_t *subbuffer = pkt->buffer + pkt->filled_size;
		ssize_t gotten_size = recv(tcp->sd, subbuffer, want_size, 0);
		if (gotten_size < 0) {
			int errno_saved = errno;
			ERR("recv failed with err %d:%s", errno_saved,
				strerror(errno_saved));
			goto error;
		}
		NOTE("TCP: Got %d bytes", gotten_size);
		if (gotten_size == 0) {
			tcp->is_closed = 1;
			if (pkt->filled_size == 0) {
				goto error;
			} else {
				break;
			}
		}

		packet_mark_received(pkt, (unsigned) gotten_size);
		want_size = packet_pending_bytes(pkt);
		NOTE("TCP: Want more %d bytes; Message %scompleted", want_size, msg->is_completed ? "" : "not ");
	}

	NOTE("TCP: Received %lu bytes", pkt->filled_size);
	return pkt;	
	 
error:
	if (pkt != NULL)
		packet_free(pkt);
	return NULL;
}
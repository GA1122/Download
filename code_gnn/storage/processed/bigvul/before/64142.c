 static int _server_handle_qAttached(libgdbr_t *g, int (*cmd_cb) (void*, const char*, char*, size_t), void *core_ptr) {
	if (send_ack (g) < 0) {
		return -1;
	}
	return send_msg (g, "0");
}
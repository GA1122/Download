static int _server_handle_vKill(libgdbr_t *g, int (*cmd_cb) (void*, const char*, char*, size_t), void *core_ptr) {
	if (send_ack (g) < 0) {
		return -1;
	}
	send_msg (g, "OK");
	return -1;
}
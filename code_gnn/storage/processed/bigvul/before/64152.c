static int _server_handle_z(libgdbr_t *g, int (*cmd_cb) (void*, const char*, char*, size_t), void *core_ptr) {
	if (send_ack (g) < 0) {
		return -1;
	}
	char set;  
	int type;
	ut64 addr;
	char cmd[64];
	sscanf (g->data, "%c%d,%"PFMT64x, &set, &type, &addr);
	if (type != 0) {
		return send_msg (g, "E01");
	}
	switch (set) {
	case 'Z':
		snprintf (cmd, sizeof (cmd) - 1, "db 0x%"PFMT64x, addr);
		break;
	case 'z':
		snprintf (cmd, sizeof (cmd) - 1, "db- 0x%"PFMT64x, addr);
		break;
	default:
		return send_msg (g, "E01");
	}
	if (cmd_cb (core_ptr, cmd, NULL, 0) < 0) {
		send_msg (g, "E01");
		return -1;
	}
	return send_msg (g, "OK");
}
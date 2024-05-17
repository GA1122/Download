send_rexec_state(int fd, struct sshbuf *conf)
{
	struct sshbuf *m;
	int r;

	debug3("%s: entering fd = %d config len %zu", __func__, fd,
	    sshbuf_len(conf));

	 
	if ((m = sshbuf_new()) == NULL)
		fatal("%s: sshbuf_new failed", __func__);
	if ((r = sshbuf_put_stringb(m, conf)) != 0)
		fatal("%s: buffer error: %s", __func__, ssh_err(r));
	if (ssh_msg_send(fd, 0, m) == -1)
		fatal("%s: ssh_msg_send failed", __func__);

	sshbuf_free(m);

	debug3("%s: done", __func__);
}

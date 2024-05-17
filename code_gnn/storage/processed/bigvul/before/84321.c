static void uncurl_attach_tls(struct uncurl_conn *ucc)
{
	ucc->ctx = ucc->tls;
	ucc->read = tls_read;
	ucc->write = tls_write;
}

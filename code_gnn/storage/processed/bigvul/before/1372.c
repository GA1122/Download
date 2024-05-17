static void nfs_umountall_req(void)
{
	uint32_t data[1024];
	uint32_t *p;
	int len;

	if (nfs_server_mount_port < 0)
		 
		return;

	p = &(data[0]);
	p = rpc_add_credentials(p);

	len = p - &(data[0]);

	rpc_req(PROG_MOUNT, MOUNT_UMOUNTALL, data, len);
}

bool smbd_vfs_init(connection_struct *conn)
{
	const char **vfs_objects;
	unsigned int i = 0;
	int j = 0;

	 
	vfs_init_default(conn);

	 
	if (conn->printer) {
		return True;
	}

	vfs_objects = lp_vfs_objects(SNUM(conn));

	 
	if (!vfs_objects || !vfs_objects[0])
		return True;

	for (i=0; vfs_objects[i] ;) {
		i++;
	}

	for (j=i-1; j >= 0; j--) {
		if (!vfs_init_custom(conn, vfs_objects[j])) {
			DEBUG(0, ("smbd_vfs_init: vfs_init_custom failed for %s\n", vfs_objects[j]));
			return False;
		}
	}
	return True;
}

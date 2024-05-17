update_zfs_shares_cb(zfs_handle_t *zhp, void *pcookie)
{
	update_cookie_t *udata = (update_cookie_t *)pcookie;
	char mountpoint[ZFS_MAXPROPLEN];
	char shareopts[ZFS_MAXPROPLEN];
	char *dataset;
	zfs_type_t type = zfs_get_type(zhp);

	if (type == ZFS_TYPE_FILESYSTEM &&
	    zfs_iter_filesystems(zhp, update_zfs_shares_cb, pcookie) != 0) {
		zfs_close(zhp);
		return (1);
	}

	if (type != ZFS_TYPE_FILESYSTEM) {
		zfs_close(zhp);
		return (0);
	}

	if (zfs_prop_get(zhp, ZFS_PROP_MOUNTPOINT, mountpoint,
	    sizeof (mountpoint), NULL, NULL, 0, B_FALSE) != 0) {
		zfs_close(zhp);
		return (0);
	}

	dataset = (char *)zfs_get_name(zhp);

	if (dataset == NULL) {
		zfs_close(zhp);
		return (0);
	}

	if (!zfs_is_mounted(zhp, NULL)) {
		zfs_close(zhp);
		return (0);
	}

	if ((udata->proto == NULL || strcmp(udata->proto, "nfs") == 0) &&
	    zfs_prop_get(zhp, ZFS_PROP_SHARENFS, shareopts,
	    sizeof (shareopts), NULL, NULL, 0, B_FALSE) == 0 &&
	    strcmp(shareopts, "off") != 0) {
		(void) process_share(udata->handle, NULL, mountpoint, NULL,
		    "nfs", shareopts, NULL, dataset, B_FALSE);
	}

	if ((udata->proto == NULL || strcmp(udata->proto, "smb") == 0) &&
	    zfs_prop_get(zhp, ZFS_PROP_SHARESMB, shareopts,
	    sizeof (shareopts), NULL, NULL, 0, B_FALSE) == 0 &&
	    strcmp(shareopts, "off") != 0) {
		(void) process_share(udata->handle, NULL, mountpoint, NULL,
		    "smb", shareopts, NULL, dataset, B_FALSE);
	}

	zfs_close(zhp);

	return (0);
}

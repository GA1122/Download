static int encode_attrs(struct xdr_stream *xdr, const struct iattr *iap, const struct nfs_server *server)
{
	char owner_name[IDMAP_NAMESZ];
	char owner_group[IDMAP_NAMESZ];
	int owner_namelen = 0;
	int owner_grouplen = 0;
	__be32 *p;
	__be32 *q;
	int len;
	uint32_t bmval0 = 0;
	uint32_t bmval1 = 0;
	int status;

	 
	len = 16;

	 
	if (iap->ia_valid & ATTR_SIZE)
		len += 8;
	if (iap->ia_valid & ATTR_MODE)
		len += 4;
	if (iap->ia_valid & ATTR_UID) {
		owner_namelen = nfs_map_uid_to_name(server->nfs_client, iap->ia_uid, owner_name);
		if (owner_namelen < 0) {
			dprintk("nfs: couldn't resolve uid %d to string\n",
					iap->ia_uid);
			 
			strcpy(owner_name, "nobody");
			owner_namelen = sizeof("nobody") - 1;
			 
		}
		len += 4 + (XDR_QUADLEN(owner_namelen) << 2);
	}
	if (iap->ia_valid & ATTR_GID) {
		owner_grouplen = nfs_map_gid_to_group(server->nfs_client, iap->ia_gid, owner_group);
		if (owner_grouplen < 0) {
			dprintk("nfs: couldn't resolve gid %d to string\n",
					iap->ia_gid);
			strcpy(owner_group, "nobody");
			owner_grouplen = sizeof("nobody") - 1;
			 
		}
		len += 4 + (XDR_QUADLEN(owner_grouplen) << 2);
	}
	if (iap->ia_valid & ATTR_ATIME_SET)
		len += 16;
	else if (iap->ia_valid & ATTR_ATIME)
		len += 4;
	if (iap->ia_valid & ATTR_MTIME_SET)
		len += 16;
	else if (iap->ia_valid & ATTR_MTIME)
		len += 4;
	RESERVE_SPACE(len);

	 
	WRITE32(2);
	q = p;
	p += 3;

	if (iap->ia_valid & ATTR_SIZE) {
		bmval0 |= FATTR4_WORD0_SIZE;
		WRITE64(iap->ia_size);
	}
	if (iap->ia_valid & ATTR_MODE) {
		bmval1 |= FATTR4_WORD1_MODE;
		WRITE32(iap->ia_mode & S_IALLUGO);
	}
	if (iap->ia_valid & ATTR_UID) {
		bmval1 |= FATTR4_WORD1_OWNER;
		WRITE32(owner_namelen);
		WRITEMEM(owner_name, owner_namelen);
	}
	if (iap->ia_valid & ATTR_GID) {
		bmval1 |= FATTR4_WORD1_OWNER_GROUP;
		WRITE32(owner_grouplen);
		WRITEMEM(owner_group, owner_grouplen);
	}
	if (iap->ia_valid & ATTR_ATIME_SET) {
		bmval1 |= FATTR4_WORD1_TIME_ACCESS_SET;
		WRITE32(NFS4_SET_TO_CLIENT_TIME);
		WRITE32(0);
		WRITE32(iap->ia_mtime.tv_sec);
		WRITE32(iap->ia_mtime.tv_nsec);
	}
	else if (iap->ia_valid & ATTR_ATIME) {
		bmval1 |= FATTR4_WORD1_TIME_ACCESS_SET;
		WRITE32(NFS4_SET_TO_SERVER_TIME);
	}
	if (iap->ia_valid & ATTR_MTIME_SET) {
		bmval1 |= FATTR4_WORD1_TIME_MODIFY_SET;
		WRITE32(NFS4_SET_TO_CLIENT_TIME);
		WRITE32(0);
		WRITE32(iap->ia_mtime.tv_sec);
		WRITE32(iap->ia_mtime.tv_nsec);
	}
	else if (iap->ia_valid & ATTR_MTIME) {
		bmval1 |= FATTR4_WORD1_TIME_MODIFY_SET;
		WRITE32(NFS4_SET_TO_SERVER_TIME);
	}
	
	 
	if (len != ((char *)p - (char *)q) + 4) {
		printk(KERN_ERR "nfs: Attr length error, %u != %Zu\n",
				len, ((char *)p - (char *)q) + 4);
		BUG();
	}
	len = (char *)p - (char *)q - 12;
	*q++ = htonl(bmval0);
	*q++ = htonl(bmval1);
	*q++ = htonl(len);

	status = 0;
 
	return status;
}
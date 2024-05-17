int nfs_map_gid_to_group(const struct nfs_server *server, kgid_t gid, char *buf, size_t buflen)
{
	struct idmap *idmap = server->nfs_client->cl_idmap;
	int ret = -EINVAL;
	__u32 id;

	id = from_kgid(&init_user_ns, gid);
	if (!(server->caps & NFS_CAP_UIDGID_NOMAP))
		ret = nfs_idmap_lookup_name(id, "group", buf, buflen, idmap);
	if (ret < 0)
		ret = nfs_map_numeric_to_string(id, buf, buflen);
	trace_nfs4_map_gid_to_group(buf, ret, id, ret);
	return ret;
}
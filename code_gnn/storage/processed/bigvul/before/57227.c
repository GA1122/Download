static int nfs4_read_done(struct rpc_task *task, struct nfs_pgio_header *hdr)
{

	dprintk("--> %s\n", __func__);

	if (!nfs4_sequence_done(task, &hdr->res.seq_res))
		return -EAGAIN;
	if (nfs4_read_stateid_changed(task, &hdr->args))
		return -EAGAIN;
	return hdr->pgio_done_cb ? hdr->pgio_done_cb(task, hdr) :
				    nfs4_read_done_cb(task, hdr);
}
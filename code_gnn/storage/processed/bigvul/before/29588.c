static void *sysvipc_proc_start(struct seq_file *s, loff_t *pos)
{
	struct ipc_proc_iter *iter = s->private;
	struct ipc_proc_iface *iface = iter->iface;
	struct ipc_ids *ids;

	ids = &iter->ns->ids[iface->ids];

	 
	down_read(&ids->rw_mutex);

	 
	if (*pos < 0)
		return NULL;

	 
	if (*pos == 0)
		return SEQ_START_TOKEN;

	 
	return sysvipc_find_ipc(ids, *pos - 1, pos);
}

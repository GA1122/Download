static struct smi_info *find_dup_si(struct smi_info *info)
{
	struct smi_info *e;

	list_for_each_entry(e, &smi_infos, link) {
		if (e->io.addr_type != info->io.addr_type)
			continue;
		if (e->io.addr_data == info->io.addr_data) {
			 
			if (info->io.slave_addr && !e->io.slave_addr)
				e->io.slave_addr = info->io.slave_addr;
			return e;
		}
	}

	return NULL;
}

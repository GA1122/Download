static void *gfi_unpack_entry(
	struct object_entry *oe,
	unsigned long *sizep)
{
	enum object_type type;
	struct packed_git *p = all_packs[oe->pack_id];
	if (p == pack_data && p->pack_size < (pack_size + 20)) {
		 
		close_pack_windows(p);
		sha1flush(pack_file);

		 
		p->pack_size = pack_size + 20;
	}
	return unpack_entry(p, oe->idx.offset, &type, sizep);
}

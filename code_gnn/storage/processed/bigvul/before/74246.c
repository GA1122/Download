struct xattr_list *get_xattr(int i, unsigned int *count, int ignore)
{
	long long start;
	struct xattr_list *xattr_list = NULL;
	unsigned int offset;
	void *xptr;
	int j = 0, res = 1;

	TRACE("get_xattr\n");

	*count = xattr_ids[i].count;
	start = SQUASHFS_XATTR_BLK(xattr_ids[i].xattr) + xattr_table_start;
	offset = SQUASHFS_XATTR_OFFSET(xattr_ids[i].xattr);
	xptr = xattrs + get_xattr_block(start) + offset;

	TRACE("get_xattr: xattr_id %d, count %d, start %lld, offset %d\n", i,
			*count, start, offset);

	while(j < *count) {
		struct squashfs_xattr_entry entry;
		struct squashfs_xattr_val val;

		if(res != 0) {
			xattr_list = realloc(xattr_list, (j + 1) *
						sizeof(struct xattr_list));
			if(xattr_list == NULL)
				MEM_ERROR();
		}
			
		SQUASHFS_SWAP_XATTR_ENTRY(xptr, &entry);
		xptr += sizeof(entry);

		res = read_xattr_entry(&xattr_list[j], &entry, xptr);
		if(ignore && res == 0) {
			 
			(*count) --;
			continue;
		}

		if(res != 1)
			goto failed;

		xptr += entry.size;
			
		TRACE("get_xattr: xattr %d, type %d, size %d, name %s\n", j,
			entry.type, entry.size, xattr_list[j].full_name); 

		if(entry.type & SQUASHFS_XATTR_VALUE_OOL) {
			long long xattr;
			void *ool_xptr;

			xptr += sizeof(val);
			SQUASHFS_SWAP_LONG_LONGS(xptr, &xattr, 1);
			xptr += sizeof(xattr);	
			start = SQUASHFS_XATTR_BLK(xattr) + xattr_table_start;
			offset = SQUASHFS_XATTR_OFFSET(xattr);
			ool_xptr = xattrs + get_xattr_block(start) + offset;
			SQUASHFS_SWAP_XATTR_VAL(ool_xptr, &val);
			xattr_list[j].value = ool_xptr + sizeof(val);
		} else {
			SQUASHFS_SWAP_XATTR_VAL(xptr, &val);
			xattr_list[j].value = xptr + sizeof(val);
			xptr += sizeof(val) + val.vsize;
		}

		TRACE("get_xattr: xattr %d, vsize %d\n", j, val.vsize);

		xattr_list[j ++].vsize = val.vsize;
	}

	if(*count == 0)
		goto failed;

	return xattr_list;

failed:
	free_xattr(xattr_list, j);

	return NULL;
}
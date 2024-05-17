static struct fwnet_fragment_info *fwnet_frag_new(
	struct fwnet_partial_datagram *pd, unsigned offset, unsigned len)
{
	struct fwnet_fragment_info *fi, *fi2, *new;
	struct list_head *list;

	list = &pd->fi_list;
	list_for_each_entry(fi, &pd->fi_list, fi_link) {
		if (fi->offset + fi->len == offset) {
			 
			 
			fi2 = list_entry(fi->fi_link.next,
					 struct fwnet_fragment_info, fi_link);
			if (fi->offset + fi->len == fi2->offset) {
				 
				fi->len += len + fi2->len;
				list_del(&fi2->fi_link);
				kfree(fi2);
			} else {
				fi->len += len;
			}

			return fi;
		}
		if (offset + len == fi->offset) {
			 
			 
			fi2 = list_entry(fi->fi_link.prev,
					 struct fwnet_fragment_info, fi_link);
			if (fi2->offset + fi2->len == fi->offset) {
				 
				fi2->len += fi->len + len;
				list_del(&fi->fi_link);
				kfree(fi);

				return fi2;
			}
			fi->offset = offset;
			fi->len += len;

			return fi;
		}
		if (offset > fi->offset + fi->len) {
			list = &fi->fi_link;
			break;
		}
		if (offset + len < fi->offset) {
			list = fi->fi_link.prev;
			break;
		}
	}

	new = kmalloc(sizeof(*new), GFP_ATOMIC);
	if (!new)
		return NULL;

	new->offset = offset;
	new->len = len;
	list_add(&new->fi_link, list);

	return new;
}

static int ocfs2_unwritten_check(struct inode *inode,
				 struct ocfs2_write_ctxt *wc,
				 struct ocfs2_write_cluster_desc *desc)
{
	struct ocfs2_inode_info *oi = OCFS2_I(inode);
	struct ocfs2_unwritten_extent *ue = NULL, *new = NULL;
	int ret = 0;

	if (!desc->c_needs_zero)
		return 0;

retry:
	spin_lock(&oi->ip_lock);
	 
	list_for_each_entry(ue, &oi->ip_unwritten_list, ue_ip_node) {
		if (desc->c_cpos == ue->ue_cpos) {
			BUG_ON(desc->c_new);
			desc->c_needs_zero = 0;
			desc->c_clear_unwritten = 0;
			goto unlock;
		}
	}

	if (wc->w_type != OCFS2_WRITE_DIRECT)
		goto unlock;

	if (new == NULL) {
		spin_unlock(&oi->ip_lock);
		new = kmalloc(sizeof(struct ocfs2_unwritten_extent),
			     GFP_NOFS);
		if (new == NULL) {
			ret = -ENOMEM;
			goto out;
		}
		goto retry;
	}
	 
	new->ue_cpos = desc->c_cpos;
	new->ue_phys = desc->c_phys;
	desc->c_clear_unwritten = 0;
	list_add_tail(&new->ue_ip_node, &oi->ip_unwritten_list);
	list_add_tail(&new->ue_node, &wc->w_unwritten_list);
	new = NULL;
unlock:
	spin_unlock(&oi->ip_lock);
out:
	if (new)
		kfree(new);
	return ret;
}
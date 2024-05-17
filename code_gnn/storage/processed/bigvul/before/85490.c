static int ocfs2_dio_end_io_write(struct inode *inode,
				  struct ocfs2_dio_write_ctxt *dwc,
				  loff_t offset,
				  ssize_t bytes)
{
	struct ocfs2_cached_dealloc_ctxt dealloc;
	struct ocfs2_extent_tree et;
	struct ocfs2_super *osb = OCFS2_SB(inode->i_sb);
	struct ocfs2_inode_info *oi = OCFS2_I(inode);
	struct ocfs2_unwritten_extent *ue = NULL;
	struct buffer_head *di_bh = NULL;
	struct ocfs2_dinode *di;
	struct ocfs2_alloc_context *data_ac = NULL;
	struct ocfs2_alloc_context *meta_ac = NULL;
	handle_t *handle = NULL;
	loff_t end = offset + bytes;
	int ret = 0, credits = 0, locked = 0;

	ocfs2_init_dealloc_ctxt(&dealloc);

	 
	if (list_empty(&dwc->dw_zero_list) &&
	    end <= i_size_read(inode) &&
	    !dwc->dw_orphaned)
		goto out;

	 
	if (dwc->dw_writer_pid != task_pid_nr(current)) {
		inode_lock(inode);
		locked = 1;
	}

	ret = ocfs2_inode_lock(inode, &di_bh, 1);
	if (ret < 0) {
		mlog_errno(ret);
		goto out;
	}

	down_write(&oi->ip_alloc_sem);

	 
	if (dwc->dw_orphaned) {
		BUG_ON(dwc->dw_writer_pid != task_pid_nr(current));

		end = end > i_size_read(inode) ? end : 0;

		ret = ocfs2_del_inode_from_orphan(osb, inode, di_bh,
				!!end, end);
		if (ret < 0)
			mlog_errno(ret);
	}

	di = (struct ocfs2_dinode *)di_bh->b_data;

	ocfs2_init_dinode_extent_tree(&et, INODE_CACHE(inode), di_bh);

	ret = ocfs2_lock_allocators(inode, &et, 0, dwc->dw_zero_count*2,
				    &data_ac, &meta_ac);
	if (ret) {
		mlog_errno(ret);
		goto unlock;
	}

	credits = ocfs2_calc_extend_credits(inode->i_sb, &di->id2.i_list);

	handle = ocfs2_start_trans(osb, credits);
	if (IS_ERR(handle)) {
		ret = PTR_ERR(handle);
		mlog_errno(ret);
		goto unlock;
	}
	ret = ocfs2_journal_access_di(handle, INODE_CACHE(inode), di_bh,
				      OCFS2_JOURNAL_ACCESS_WRITE);
	if (ret) {
		mlog_errno(ret);
		goto commit;
	}

	list_for_each_entry(ue, &dwc->dw_zero_list, ue_node) {
		ret = ocfs2_mark_extent_written(inode, &et, handle,
						ue->ue_cpos, 1,
						ue->ue_phys,
						meta_ac, &dealloc);
		if (ret < 0) {
			mlog_errno(ret);
			break;
		}
	}

	if (end > i_size_read(inode)) {
		ret = ocfs2_set_inode_size(handle, inode, di_bh, end);
		if (ret < 0)
			mlog_errno(ret);
	}
commit:
	ocfs2_commit_trans(osb, handle);
unlock:
	up_write(&oi->ip_alloc_sem);
	ocfs2_inode_unlock(inode, 1);
	brelse(di_bh);
out:
	if (data_ac)
		ocfs2_free_alloc_context(data_ac);
	if (meta_ac)
		ocfs2_free_alloc_context(meta_ac);
	ocfs2_run_deallocs(osb, &dealloc);
	if (locked)
		inode_unlock(inode);
	ocfs2_dio_free_write_ctx(inode, dwc);

	return ret;
}
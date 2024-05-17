smb_ofile_close(smb_ofile_t *of, int32_t mtime_sec)
{
	smb_attr_t *pa;
	timestruc_t now;
	uint32_t flags = 0;

	SMB_OFILE_VALID(of);

	mutex_enter(&of->f_mutex);
	ASSERT(of->f_refcnt);
	if (of->f_state != SMB_OFILE_STATE_OPEN) {
		mutex_exit(&of->f_mutex);
		return;
	}
	of->f_state = SMB_OFILE_STATE_CLOSING;
	mutex_exit(&of->f_mutex);

	switch (of->f_ftype) {
	case SMB_FTYPE_BYTE_PIPE:
	case SMB_FTYPE_MESG_PIPE:
		smb_opipe_close(of);
		smb_server_dec_pipes(of->f_server);
		break;

	case SMB_FTYPE_DISK:
	case SMB_FTYPE_PRINTER:
		 
		pa = &of->f_pending_attr;
		if (mtime_sec != 0) {
			pa->sa_vattr.va_mtime.tv_sec = mtime_sec;
			pa->sa_mask |= SMB_AT_MTIME;
		}

		 
		if (of->f_written &&
		    (pa->sa_mask & SMB_AT_MTIME) == 0) {
			pa->sa_mask |= SMB_AT_MTIME;
			gethrestime(&now);
			pa->sa_vattr.va_mtime = now;
		}

		if (of->f_flags & SMB_OFLAGS_SET_DELETE_ON_CLOSE) {
			if (smb_tree_has_feature(of->f_tree,
			    SMB_TREE_CATIA)) {
				flags |= SMB_CATIA;
			}
			(void) smb_node_set_delete_on_close(of->f_node,
			    of->f_cr, flags);
		}
		smb_fsop_unshrlock(of->f_cr, of->f_node, of->f_uniqid);
		smb_node_destroy_lock_by_ofile(of->f_node, of);

		if (smb_node_is_file(of->f_node)) {
			(void) smb_fsop_close(of->f_node, of->f_mode,
			    of->f_cr);
			smb_oplock_release(of->f_node, of);
		} else {
			 
			if (of->f_odir != NULL)
				smb_odir_close(of->f_odir);
		}
		if (smb_node_dec_open_ofiles(of->f_node) == 0) {
			 
			pa->sa_dosattr =
			    of->f_node->n_pending_dosattr;
			if (pa->sa_dosattr != 0)
				pa->sa_mask |= SMB_AT_DOSATTR;
			 
			of->f_node->n_allocsz = 0;
		}
		if (pa->sa_mask != 0) {
			 
			(void) smb_node_setattr(NULL, of->f_node,
			    of->f_cr, NULL, pa);
		}

		 
		if (of->f_node->n_fcn.fcn_count)
			smb_notify_file_closed(of);

		smb_server_dec_files(of->f_server);
		break;
	}
	atomic_dec_32(&of->f_tree->t_open_files);

	mutex_enter(&of->f_mutex);
	ASSERT(of->f_refcnt);
	ASSERT(of->f_state == SMB_OFILE_STATE_CLOSING);
	of->f_state = SMB_OFILE_STATE_CLOSED;
	mutex_exit(&of->f_mutex);
}

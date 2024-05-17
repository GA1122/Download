smb_ofile_open(
    smb_request_t	*sr,
    smb_node_t		*node,
    struct open_param	*op,
    uint16_t		ftype,
    uint32_t		uniqid,
    smb_error_t		*err)
{
	smb_tree_t	*tree = sr->tid_tree;
	smb_ofile_t	*of;
	uint16_t	fid;
	smb_attr_t	attr;
	int		rc;
	enum errstates { EMPTY, FIDALLOC, CRHELD, MUTEXINIT };
	enum errstates	state = EMPTY;

	if (smb_idpool_alloc(&tree->t_fid_pool, &fid)) {
		err->status = NT_STATUS_TOO_MANY_OPENED_FILES;
		err->errcls = ERRDOS;
		err->errcode = ERROR_TOO_MANY_OPEN_FILES;
		return (NULL);
	}
	state = FIDALLOC;

	of = kmem_cache_alloc(smb_cache_ofile, KM_SLEEP);
	bzero(of, sizeof (smb_ofile_t));
	of->f_magic = SMB_OFILE_MAGIC;
	of->f_refcnt = 1;
	of->f_fid = fid;
	of->f_uniqid = uniqid;
	of->f_opened_by_pid = sr->smb_pid;
	of->f_granted_access = op->desired_access;
	of->f_share_access = op->share_access;
	of->f_create_options = op->create_options;
	of->f_cr = (op->create_options & FILE_OPEN_FOR_BACKUP_INTENT) ?
	    smb_user_getprivcred(sr->uid_user) : sr->uid_user->u_cred;
	crhold(of->f_cr);
	state = CRHELD;
	of->f_ftype = ftype;
	of->f_server = tree->t_server;
	of->f_session = tree->t_session;
	 
	smb_user_hold_internal(sr->uid_user);
	of->f_user = sr->uid_user;
	of->f_tree = tree;
	of->f_node = node;

	mutex_init(&of->f_mutex, NULL, MUTEX_DEFAULT, NULL);
	state = MUTEXINIT;
	of->f_state = SMB_OFILE_STATE_OPEN;

	if (ftype == SMB_FTYPE_MESG_PIPE) {
		 
		of->f_pipe = op->pipe;
		smb_server_inc_pipes(of->f_server);
	} else {
		ASSERT(ftype == SMB_FTYPE_DISK);  
		ASSERT(node);

		 
		if ((of->f_granted_access & FILE_DATA_ALL) == FILE_EXECUTE)
			of->f_flags |= SMB_OFLAGS_EXECONLY;

		bzero(&attr, sizeof (smb_attr_t));
		attr.sa_mask = SMB_AT_UID | SMB_AT_DOSATTR;
		rc = smb_node_getattr(NULL, node, of->f_cr, NULL, &attr);
		if (rc != 0) {
			err->status = NT_STATUS_INTERNAL_ERROR;
			err->errcls = ERRDOS;
			err->errcode = ERROR_INTERNAL_ERROR;
			goto errout;
		}
		if (crgetuid(of->f_cr) == attr.sa_vattr.va_uid) {
			 
			of->f_granted_access |= FILE_READ_ATTRIBUTES;
		}

		if (smb_node_is_file(node)) {
			of->f_mode =
			    smb_fsop_amask_to_omode(of->f_granted_access);
			if (smb_fsop_open(node, of->f_mode, of->f_cr) != 0) {
				err->status = NT_STATUS_ACCESS_DENIED;
				err->errcls = ERRDOS;
				err->errcode = ERROR_ACCESS_DENIED;
				goto errout;
			}
		}

		if (tree->t_flags & SMB_TREE_READONLY)
			of->f_flags |= SMB_OFLAGS_READONLY;

		 
		if (attr.sa_dosattr & FILE_ATTRIBUTE_READONLY)
			of->f_flags |= SMB_OFLAGS_READONLY;

		smb_node_inc_open_ofiles(node);
		smb_node_add_ofile(node, of);
		smb_node_ref(node);
		smb_server_inc_files(of->f_server);
	}
	smb_llist_enter(&tree->t_ofile_list, RW_WRITER);
	smb_llist_insert_tail(&tree->t_ofile_list, of);
	smb_llist_exit(&tree->t_ofile_list);
	atomic_inc_32(&tree->t_open_files);
	atomic_inc_32(&of->f_session->s_file_cnt);
	return (of);

errout:
	switch (state) {
	case MUTEXINIT:
		mutex_destroy(&of->f_mutex);
		smb_user_release(of->f_user);
		 
	case CRHELD:
		crfree(of->f_cr);
		of->f_magic = 0;
		kmem_cache_free(smb_cache_ofile, of);
		 
	case FIDALLOC:
		smb_idpool_free(&tree->t_fid_pool, fid);
		 
	case EMPTY:
		break;
	}
	return (NULL);
}
bool vfs_init_custom(connection_struct *conn, const char *vfs_object)
{
	char *module_path = NULL;
	char *module_name = NULL;
	char *module_param = NULL, *p;
	vfs_handle_struct *handle;
	const struct vfs_init_function_entry *entry;

	if (!conn||!vfs_object||!vfs_object[0]) {
		DEBUG(0, ("vfs_init_custom() called with NULL pointer or "
			  "empty vfs_object!\n"));
		return False;
	}

	if(!backends) {
		static_init_vfs;
	}

	DEBUG(3, ("Initialising custom vfs hooks from [%s]\n", vfs_object));

	module_path = smb_xstrdup(vfs_object);

	p = strchr_m(module_path, ':');

	if (p) {
		*p = 0;
		module_param = p+1;
		trim_char(module_param, ' ', ' ');
	}

	trim_char(module_path, ' ', ' ');

	module_name = smb_xstrdup(module_path);

	if ((module_name[0] == '/') &&
	    (strcmp(module_path, DEFAULT_VFS_MODULE_NAME) != 0)) {

		 

		SAFE_FREE(module_name);
		module_name = smb_xstrdup(strrchr_m(module_path, '/')+1);

		p = strchr_m(module_name, '.');

		if (p != NULL) {
			*p = '\0';
		}
	}

	 
	entry = vfs_find_backend_entry(module_name);
	if (!entry) {
		NTSTATUS status;

		DEBUG(5, ("vfs module [%s] not loaded - trying to load...\n",
			  vfs_object));

		status = smb_load_module("vfs", module_path);
		if (!NT_STATUS_IS_OK(status)) {
			DEBUG(0, ("error probing vfs module '%s': %s\n",
				  module_path, nt_errstr(status)));
			goto fail;
		}

		entry = vfs_find_backend_entry(module_name);
		if (!entry) {
			DEBUG(0,("Can't find a vfs module [%s]\n",vfs_object));
			goto fail;
		}
	}

	DEBUGADD(5,("Successfully loaded vfs module [%s] with the new modules system\n", vfs_object));

	handle = talloc_zero(conn, vfs_handle_struct);
	if (!handle) {
		DEBUG(0,("TALLOC_ZERO() failed!\n"));
		goto fail;
	}
	handle->conn = conn;
	handle->fns = entry->fns;
	if (module_param) {
		handle->param = talloc_strdup(conn, module_param);
	}
	DLIST_ADD(conn->vfs_handles, handle);

	SAFE_FREE(module_path);
	SAFE_FREE(module_name);
	return True;

 fail:
	SAFE_FREE(module_path);
	SAFE_FREE(module_name);
	return False;
}

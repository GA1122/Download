NTSTATUS cli_resolve_path(TALLOC_CTX *ctx,
			  const char *mountpt,
			  const struct user_auth_info *dfs_auth_info,
			  struct cli_state *rootcli,
			  const char *path,
			  struct cli_state **targetcli,
			  char **pp_targetpath)
{
	struct client_dfs_referral *refs = NULL;
	size_t num_refs = 0;
	size_t consumed = 0;
	struct cli_state *cli_ipc = NULL;
	char *dfs_path = NULL;
	char *cleanpath = NULL;
	char *extrapath = NULL;
	int pathlen;
	struct cli_state *newcli = NULL;
	struct cli_state *ccli = NULL;
	int count = 0;
	char *newpath = NULL;
	char *newmount = NULL;
	char *ppath = NULL;
	SMB_STRUCT_STAT sbuf;
	uint32_t attributes;
	NTSTATUS status;
	struct smbXcli_tcon *root_tcon = NULL;
	struct smbXcli_tcon *target_tcon = NULL;
	struct cli_dfs_path_split *dfs_refs = NULL;

	if ( !rootcli || !path || !targetcli ) {
		return NT_STATUS_INVALID_PARAMETER;
	}

	 

	if (smbXcli_conn_protocol(rootcli->conn) >= PROTOCOL_SMB2_02) {
		root_tcon = rootcli->smb2.tcon;
	} else {
		root_tcon = rootcli->smb1.tcon;
	}

	if (!smbXcli_tcon_is_dfs_share(root_tcon)) {
		*targetcli = rootcli;
		*pp_targetpath = talloc_strdup(ctx, path);
		if (!*pp_targetpath) {
			return NT_STATUS_NO_MEMORY;
		}
		return NT_STATUS_OK;
	}

	*targetcli = NULL;

	 

	cleanpath = clean_path(ctx, path);
	if (!cleanpath) {
		return NT_STATUS_NO_MEMORY;
	}

	dfs_path = cli_dfs_make_full_path(ctx, rootcli, cleanpath);
	if (!dfs_path) {
		return NT_STATUS_NO_MEMORY;
	}

	status = cli_qpathinfo_basic( rootcli, dfs_path, &sbuf, &attributes);
	if (NT_STATUS_IS_OK(status)) {
		 
		*targetcli = rootcli;
		*pp_targetpath = talloc_strdup(ctx, path);
		if (!*pp_targetpath) {
			return NT_STATUS_NO_MEMORY;
		}
		goto done;
	}

	 

	if (cli_dfs_check_error(rootcli, NT_STATUS_OBJECT_NAME_NOT_FOUND,
				status)) {
		*targetcli = rootcli;
		*pp_targetpath = talloc_strdup(ctx, path);
		if (!*pp_targetpath) {
			return NT_STATUS_NO_MEMORY;
		}
		goto done;
	}

	 

	if (!cli_dfs_check_error(rootcli, NT_STATUS_PATH_NOT_COVERED,
				 status)) {
		return status;
	}

	 

	status = cli_cm_open(ctx,
			     rootcli,
			     smbXcli_conn_remote_name(rootcli->conn),
			     "IPC$",
			     dfs_auth_info,
			     false,
			     smb1cli_conn_encryption_on(rootcli->conn),
			     smbXcli_conn_protocol(rootcli->conn),
			     0,
			     0x20,
			     &cli_ipc);
	if (!NT_STATUS_IS_OK(status)) {
		return status;
	}

	status = cli_dfs_get_referral(ctx, cli_ipc, dfs_path, &refs,
				      &num_refs, &consumed);
	if (!NT_STATUS_IS_OK(status)) {
		return status;
	}

	if (!num_refs || !refs[0].dfspath) {
		return NT_STATUS_NOT_FOUND;
	}

	 
	dfs_refs = talloc_array(ctx, struct cli_dfs_path_split, num_refs);
	if (dfs_refs == NULL) {
		return NT_STATUS_NO_MEMORY;
	}

	for (count = 0; count < num_refs; count++) {
		if (!split_dfs_path(dfs_refs, refs[count].dfspath,
				    &dfs_refs[count].server,
				    &dfs_refs[count].share,
				    &dfs_refs[count].extrapath)) {
			TALLOC_FREE(dfs_refs);
			return NT_STATUS_NOT_FOUND;
		}

		ccli = cli_cm_find(rootcli, dfs_refs[count].server,
				   dfs_refs[count].share);
		if (ccli != NULL) {
			extrapath = dfs_refs[count].extrapath;
			*targetcli = ccli;
			break;
		}
	}

	 
	for (count = 0; (ccli == NULL) && (count < num_refs); count++) {
		 
		status = cli_cm_connect(ctx, rootcli,
				dfs_refs[count].server,
				dfs_refs[count].share,
				dfs_auth_info,
				false,
				smb1cli_conn_encryption_on(rootcli->conn),
				smbXcli_conn_protocol(rootcli->conn),
				0,
				0x20,
				targetcli);
		if (!NT_STATUS_IS_OK(status)) {
			d_printf("Unable to follow dfs referral [\\%s\\%s]\n",
				 dfs_refs[count].server,
				 dfs_refs[count].share);
			continue;
		} else {
			extrapath = dfs_refs[count].extrapath;
			break;
		}
	}

	 
	if (*targetcli == NULL) {
		TALLOC_FREE(dfs_refs);
		return status;
	}

	 

	dfs_path = cli_dfs_make_full_path(ctx, rootcli, path);
	if (!dfs_path) {
		TALLOC_FREE(dfs_refs);
		return NT_STATUS_NO_MEMORY;
	}
	pathlen = strlen(dfs_path);
	consumed = MIN(pathlen, consumed);
	*pp_targetpath = talloc_strdup(ctx, &dfs_path[consumed]);
	if (!*pp_targetpath) {
		TALLOC_FREE(dfs_refs);
		return NT_STATUS_NO_MEMORY;
	}
	dfs_path[consumed] = '\0';

	 

	if (extrapath && strlen(extrapath) > 0) {
		 
		 
		if (extrapath[strlen(extrapath)-1] != '\\' && **pp_targetpath != '\\') {
			*pp_targetpath = talloc_asprintf(ctx,
						  "%s\\%s",
						  extrapath,
						  *pp_targetpath);
		} else {
			*pp_targetpath = talloc_asprintf(ctx,
						  "%s%s",
						  extrapath,
						  *pp_targetpath);
		}
		if (!*pp_targetpath) {
			TALLOC_FREE(dfs_refs);
			return NT_STATUS_NO_MEMORY;
		}
	}

	 
	 

	ppath = dfs_path;

	if (*ppath != '\\') {
		d_printf("cli_resolve_path: "
			"dfs_path (%s) not in correct format.\n",
			dfs_path );
		TALLOC_FREE(dfs_refs);
		return NT_STATUS_NOT_FOUND;
	}

	ppath++;  

	if ((ppath = strchr_m( dfs_path, '\\' )) == NULL) {
		TALLOC_FREE(dfs_refs);
		return NT_STATUS_NOT_FOUND;
	}

	ppath++;  

	if ((ppath = strchr_m( ppath+1, '\\' )) == NULL) {
		TALLOC_FREE(dfs_refs);
		return NT_STATUS_NOT_FOUND;
	}

	ppath++;  

	newmount = talloc_asprintf(ctx, "%s\\%s", mountpt, ppath );
	if (!newmount) {
		TALLOC_FREE(dfs_refs);
		return NT_STATUS_NOT_FOUND;
	}

	cli_set_mntpoint(*targetcli, newmount);

	 

	if (!strequal(*pp_targetpath, "\\") && !strequal(*pp_targetpath, "/")) {
		status = cli_resolve_path(ctx,
					  newmount,
					  dfs_auth_info,
					  *targetcli,
					  *pp_targetpath,
					  &newcli,
					  &newpath);
		if (NT_STATUS_IS_OK(status)) {
			 
			*targetcli = newcli;
			*pp_targetpath = newpath;
			TALLOC_FREE(dfs_refs);
			return status;
		}
	}

  done:

	if (smbXcli_conn_protocol((*targetcli)->conn) >= PROTOCOL_SMB2_02) {
		target_tcon = (*targetcli)->smb2.tcon;
	} else {
		target_tcon = (*targetcli)->smb1.tcon;
	}

	 
	if (smbXcli_tcon_is_dfs_share(target_tcon)) {
		dfs_path = talloc_strdup(ctx, *pp_targetpath);
		if (!dfs_path) {
			TALLOC_FREE(dfs_refs);
			return NT_STATUS_NO_MEMORY;
		}
		*pp_targetpath = cli_dfs_make_full_path(ctx, *targetcli, dfs_path);
		if (*pp_targetpath == NULL) {
			TALLOC_FREE(dfs_refs);
			return NT_STATUS_NO_MEMORY;
		}
	}

	TALLOC_FREE(dfs_refs);
	return NT_STATUS_OK;
}

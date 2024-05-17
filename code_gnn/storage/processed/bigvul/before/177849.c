 SMBC_server_internal(TALLOC_CTX *ctx,
             SMBCCTX *context,
             bool connect_if_not_found,
             const char *server,
             uint16_t port,
             const char *share,
             char **pp_workgroup,
             char **pp_username,
             char **pp_password,
 	    bool *in_cache)
 {
 	SMBCSRV *srv=NULL;
 	char *workgroup = NULL;
 	struct cli_state *c = NULL;
 	const char *server_n = server;
         int is_ipc = (share != NULL && strcmp(share, "IPC$") == 0);
 	uint32_t fs_attrs = 0;
         const char *username_used;
  	NTSTATUS status;
         char *newserver, *newshare;
         int flags = 0;
         struct smbXcli_tcon *tcon = NULL;
//        int signing_state = SMB_SIGNING_DEFAULT;
  
         ZERO_STRUCT(c);
         *in_cache = false;
 	if (server[0] == 0) {
 		errno = EPERM;
 		return NULL;
 	}
 
          
         srv = SMBC_find_server(ctx, context, server, share,
                                pp_workgroup, pp_username, pp_password);
 
          
         if (srv &&
 	    share != NULL && *share != '\0' &&
             smbc_getOptionOneSharePerServer(context)) {
 
                  
 
 		 
                 if (!cli_state_has_tcon(srv->cli)) {
                          
 			SMBC_call_auth_fn(ctx, context,
 					  smbXcli_conn_remote_name(srv->cli->conn),
 					  srv->cli->share,
                                           pp_workgroup,
                                           pp_username,
                                           pp_password);
 
 			if (!*pp_workgroup || !*pp_username || !*pp_password) {
 				errno = ENOMEM;
 				cli_shutdown(srv->cli);
 				srv->cli = NULL;
 				smbc_getFunctionRemoveCachedServer(context)(context,
                                                                             srv);
 				return NULL;
 			}
 
 			 
 
 			status = cli_tree_connect(srv->cli,
 						  srv->cli->share,
 						  "?????",
 						  *pp_password,
 						  strlen(*pp_password)+1);
 			if (!NT_STATUS_IS_OK(status)) {
                                 errno = map_errno_from_nt_status(status);
                                 cli_shutdown(srv->cli);
 				srv->cli = NULL;
                                 smbc_getFunctionRemoveCachedServer(context)(context,
                                                                             srv);
                                 srv = NULL;
                         }
 
                          
                         if (is_ipc) {
                                 DEBUG(4,
                                       ("IPC$ so ignore case sensitivity\n"));
                                 status = NT_STATUS_OK;
                         } else {
                                 status = cli_get_fs_attr_info(c, &fs_attrs);
                         }
 
                         if (!NT_STATUS_IS_OK(status)) {
                                 DEBUG(4, ("Could not retrieve "
                                           "case sensitivity flag: %s.\n",
                                           nt_errstr(status)));
 
                                  
                                 if (smbc_getOptionCaseSensitive(context)) {
                                         cli_set_case_sensitive(c, True);
                                 } else {
                                         cli_set_case_sensitive(c, False);
                                 }
                         } else if (!is_ipc) {
                                 DEBUG(4,
                                       ("Case sensitive: %s\n",
                                        (fs_attrs & FILE_CASE_SENSITIVE_SEARCH
                                         ? "True"
                                         : "False")));
                                 cli_set_case_sensitive(
                                         c,
                                         (fs_attrs & FILE_CASE_SENSITIVE_SEARCH
                                          ? True
                                          : False));
                         }
 
                          
                         if (srv) {
 				const char *remote_name =
 					smbXcli_conn_remote_name(srv->cli->conn);
 
 				srv->dev = (dev_t)(str_checksum(remote_name) ^
                                                    str_checksum(srv->cli->share));
                         }
                 }
         }
 
          
         if (srv) {
 
                  
 		*in_cache = true;
                 goto done;
         }
 
          
         if (! connect_if_not_found) {
                  
                 return NULL;
         }
 
 	if (!*pp_workgroup || !*pp_username || !*pp_password) {
 		errno = ENOMEM;
 		return NULL;
 	}
 
 	DEBUG(4,("SMBC_server: server_n=[%s] server=[%s]\n", server_n, server));
 
 	DEBUG(4,(" -> server_n=[%s] server=[%s]\n", server_n, server));
 
 	status = NT_STATUS_UNSUCCESSFUL;
 
 	if (smbc_getOptionUseKerberos(context)) {
 		flags |= CLI_FULL_CONNECTION_USE_KERBEROS;
 	}
 
 	if (smbc_getOptionFallbackAfterKerberos(context)) {
 		flags |= CLI_FULL_CONNECTION_FALLBACK_AFTER_KERBEROS;
 	}
 
 	if (smbc_getOptionUseCCache(context)) {
 		flags |= CLI_FULL_CONNECTION_USE_CCACHE;
 	}
 
 	if (smbc_getOptionUseNTHash(context)) {
 		flags |= CLI_FULL_CONNECTION_USE_NT_HASH;
                 flags |= CLI_FULL_CONNECTION_USE_NT_HASH;
         }
  
//        if (context->internal->smb_encryption_level != SMBC_ENCRYPTLEVEL_NONE) {
//                signing_state = SMB_SIGNING_REQUIRED;
//        }
// 
         if (port == 0) {
                 if (share == NULL || *share == '\0' || is_ipc) {
                          
                         status = cli_connect_nb(server_n, NULL, NBT_SMB_PORT, 0x20,
                                         smbc_getNetbiosName(context),
                                       SMB_SIGNING_DEFAULT, flags, &c);
//                                        signing_state, flags, &c);
                 }
         }
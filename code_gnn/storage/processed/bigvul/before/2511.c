SMBC_find_server(TALLOC_CTX *ctx,
                 SMBCCTX *context,
                 const char *server,
                 const char *share,
                 char **pp_workgroup,
                 char **pp_username,
                 char **pp_password)
{
        SMBCSRV *srv;
        int auth_called = 0;

        if (!pp_workgroup || !pp_username || !pp_password) {
                return NULL;
        }

check_server_cache:

	srv = smbc_getFunctionGetCachedServer(context)(context,
                                                       server, share,
                                                       *pp_workgroup,
                                                       *pp_username);

	if (!auth_called && !srv && (!*pp_username || !(*pp_username)[0] ||
                                     !*pp_password || !(*pp_password)[0])) {
		SMBC_call_auth_fn(ctx, context, server, share,
                                  pp_workgroup, pp_username, pp_password);

		 
		auth_called = 1;
		goto check_server_cache;

	}

	if (srv) {
		if (smbc_getFunctionCheckServer(context)(context, srv)) {
			 
			if (smbc_getFunctionRemoveUnusedServer(context)(context,
                                                                        srv)) {
                                 
				smbc_getFunctionRemoveCachedServer(context)(context,
                                                                            srv);
			}

			 
			goto check_server_cache;
		}

		return srv;
 	}

        return NULL;
}

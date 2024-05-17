static int handle_NPP_Destroy(rpc_connection_t *connection)
{
  D(bug("handle_NPP_Destroy\n"));

  int error;
  PluginInstance *plugin;
  error = rpc_method_get_args(connection,
							  RPC_TYPE_NPW_PLUGIN_INSTANCE, &plugin,
							  RPC_TYPE_INVALID);

  if (error != RPC_ERROR_NO_ERROR) {
	npw_perror("NPP_Destroy() get args", error);
	return error;
  }

  NPSavedData *save_area = NULL;
  NPError ret = NPERR_NO_ERROR;
   
  rpc_connection_ref(connection);
  if (!rpc_method_in_invoke(connection)) {
	 
	D(bug("NPP_Destroy is fine.\n"));
	ret = g_NPP_Destroy_Now(plugin, &save_area);
  } else {
	 
    D(bug("NPP_Destroy raced; delaying it to get a clean stack.\n"));
	delayed_destroys_add(plugin);
  }

  error = rpc_method_send_reply(connection,
								RPC_TYPE_INT32, ret,
								RPC_TYPE_NP_SAVED_DATA, save_area,
								RPC_TYPE_INVALID);
  if (save_area) {
    if (save_area->buf)
      NPN_MemFree(save_area->buf);
    NPN_MemFree(save_area);
  }

  rpc_connection_unref(connection);
  return error;
}

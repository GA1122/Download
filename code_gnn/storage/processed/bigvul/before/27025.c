static gboolean delayed_calls_process(PluginInstance *plugin, gboolean is_in_NPP_Destroy)
{
  while (g_delayed_calls != NULL) {

	if (!is_in_NPP_Destroy) {
	   
	  if (rpc_wait_dispatch(g_rpc_connection, 0) > 0)
		return TRUE;
	}

	DelayedCall *dcall = (DelayedCall *)g_delayed_calls->data;
	 
	g_delayed_calls = g_list_delete_link(g_delayed_calls, g_delayed_calls);
	switch (dcall->type) {
	case RPC_DELAYED_NPN_RELEASE_OBJECT:
	  {
		NPObject *npobj = (NPObject *)dcall->data;
		g_NPN_ReleaseObject_Now(npobj);
		break;
	  }
	}
	NPW_MemFree(dcall);
  }

  if (g_delayed_calls)
	return TRUE;

  if (g_delayed_calls_id) {
	g_source_remove(g_delayed_calls_id);
	g_delayed_calls_id = 0;
  }
  return FALSE;
}
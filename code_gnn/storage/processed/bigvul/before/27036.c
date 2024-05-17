g_NPN_DestroyStream(NPP instance, NPStream *stream, NPError reason)
{
  if (!thread_check()) {
	npw_printf("WARNING: NPN_DestroyStream not called from the main thread\n");
	return NPERR_INVALID_INSTANCE_ERROR;
  }
  
  if (instance == NULL)
	return NPERR_INVALID_INSTANCE_ERROR;

  PluginInstance *plugin = PLUGIN_INSTANCE(instance);
  if (plugin == NULL)
	return NPERR_INVALID_INSTANCE_ERROR;

  if (stream == NULL)
	return NPERR_INVALID_PARAM;

  D(bugiI("NPN_DestroyStream instance=%p, stream=%p, reason=%s\n",
		instance, stream, string_of_NPReason(reason)));
  npw_plugin_instance_ref(plugin);
  NPError ret = invoke_NPN_DestroyStream(plugin, stream, reason);
  npw_plugin_instance_unref(plugin);
  D(bugiD("NPN_DestroyStream return: %d [%s]\n", ret, string_of_NPError(ret)));

  StreamInstance *stream_ndata = stream->ndata;
  if (stream_ndata && stream_ndata->is_plugin_stream) {
	id_remove(stream_ndata->stream_id);
	free(stream_ndata);
	free((char *)stream->url);
	free((char *)stream->headers);
	free(stream);
  }

  return ret;
}
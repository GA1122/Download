static int do_main(int argc, char **argv, const char *connection_path)
{
  if (do_test() != 0)
	return 1;
  if (connection_path == NULL) {
	npw_printf("ERROR: missing connection path argument\n");
	return 1;
  }
  D(bug("  Plugin connection: %s\n", connection_path));
  D(bug("  Plugin viewer pid: %d\n", getpid()));

  thread_check_init();
  D(bug("  Plugin main thread: %p\n", g_main_thread));

#if defined(__linux__)
  if (getenv("LD_PRELOAD"))
	unsetenv("LD_PRELOAD");
#endif

  XtToolkitInitialize();
  x_app_context = XtCreateApplicationContext();
  x_display = XtOpenDisplay(x_app_context, NULL, "npw-viewer", "npw-viewer", NULL, 0, &argc, argv);
  g_thread_init(NULL);
  gtk_init(&argc, &argv);

  if ((g_rpc_connection = rpc_init_server(connection_path)) == NULL) {
	npw_printf("ERROR: failed to initialize plugin-side RPC server connection\n");
	return 1;
  }
  if (rpc_add_np_marshalers(g_rpc_connection) < 0) {
	npw_printf("ERROR: failed to initialize plugin-side marshalers\n");
	return 1;
  }
  static const rpc_method_descriptor_t vtable[] = {
	{ RPC_METHOD_NP_GET_MIME_DESCRIPTION,		handle_NP_GetMIMEDescription },
	{ RPC_METHOD_NP_GET_VALUE,					handle_NP_GetValue },
	{ RPC_METHOD_NP_INITIALIZE,					handle_NP_Initialize },
	{ RPC_METHOD_NP_SHUTDOWN,					handle_NP_Shutdown },
	{ RPC_METHOD_NPP_NEW,						handle_NPP_New },
	{ RPC_METHOD_NPP_DESTROY,					handle_NPP_Destroy },
	{ RPC_METHOD_NPP_GET_VALUE,					handle_NPP_GetValue },
	{ RPC_METHOD_NPP_SET_WINDOW,				handle_NPP_SetWindow },
	{ RPC_METHOD_NPP_URL_NOTIFY,				handle_NPP_URLNotify },
	{ RPC_METHOD_NPP_NEW_STREAM,				handle_NPP_NewStream },
	{ RPC_METHOD_NPP_DESTROY_STREAM,			handle_NPP_DestroyStream },
	{ RPC_METHOD_NPP_WRITE_READY,				handle_NPP_WriteReady },
	{ RPC_METHOD_NPP_WRITE,						handle_NPP_Write },
	{ RPC_METHOD_NPP_STREAM_AS_FILE,			handle_NPP_StreamAsFile },
	{ RPC_METHOD_NPP_PRINT,						handle_NPP_Print },
	{ RPC_METHOD_NPP_HANDLE_EVENT,				handle_NPP_HandleEvent },
	{ RPC_METHOD_NPCLASS_INVALIDATE,			npclass_handle_Invalidate },
	{ RPC_METHOD_NPCLASS_HAS_METHOD,			npclass_handle_HasMethod },
	{ RPC_METHOD_NPCLASS_INVOKE,				npclass_handle_Invoke },
	{ RPC_METHOD_NPCLASS_INVOKE_DEFAULT,		npclass_handle_InvokeDefault },
	{ RPC_METHOD_NPCLASS_HAS_PROPERTY,			npclass_handle_HasProperty },
	{ RPC_METHOD_NPCLASS_GET_PROPERTY,			npclass_handle_GetProperty },
	{ RPC_METHOD_NPCLASS_SET_PROPERTY,			npclass_handle_SetProperty },
	{ RPC_METHOD_NPCLASS_REMOVE_PROPERTY,		npclass_handle_RemoveProperty },
  };
  if (rpc_connection_add_method_descriptors(g_rpc_connection, vtable, sizeof(vtable) / sizeof(vtable[0])) < 0) {
	npw_printf("ERROR: failed to setup NPP method callbacks\n");
	return 1;
  }

  id_init();

  GSource *rpc_source = g_source_new(&rpc_event_funcs, sizeof(GSource));
  if (rpc_source == NULL) {
	npw_printf("ERROR: failed to initialize plugin-side RPC events listener\n");
	return 1;
  }
  g_source_set_priority(rpc_source, G_PRIORITY_LOW);
  g_source_attach(rpc_source, NULL);
  rpc_event_poll_fd.fd = rpc_listen_socket(g_rpc_connection);
  rpc_event_poll_fd.events = G_IO_IN;
  rpc_event_poll_fd.revents = 0;
  g_source_set_callback(rpc_source, (GSourceFunc)rpc_dispatch, g_rpc_connection, NULL);
  g_source_add_poll(rpc_source, &rpc_event_poll_fd);

  rpc_connection_set_error_callback(g_rpc_connection, rpc_error_callback_cb, NULL);
 
  gtk_main();
  D(bug("--- EXIT ---\n"));

#if USE_NPIDENTIFIER_CACHE
  npidentifier_cache_destroy();
#endif

  g_source_destroy(rpc_source);
  if (xt_source)
	g_source_destroy(xt_source);

  if (g_user_agent)
	free(g_user_agent);
  if (g_rpc_connection)
	rpc_connection_unref(g_rpc_connection);

  id_kill();
  return 0;
}
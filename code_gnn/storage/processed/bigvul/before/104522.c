void ExtensionFunctionDispatcher::DispatchOnIOThread(
    const ExtensionInfoMap* extension_info_map,
    ProfileId profile_id,
    int render_process_id,
    base::WeakPtr<ChromeRenderMessageFilter> ipc_sender,
    int routing_id,
    const ExtensionHostMsg_Request_Params& params) {
  const Extension* extension =
      extension_info_map->extensions().GetByURL(params.source_url);

  scoped_refptr<ExtensionFunction> function(
      CreateExtensionFunction(params, extension, profile_id, render_process_id,
                              ipc_sender, routing_id));
  if (!function)
    return;

  IOThreadExtensionFunction* function_io =
      function->AsIOThreadExtensionFunction();
  if (!function_io) {
    NOTREACHED();
    return;
  }
  function_io->set_ipc_sender(ipc_sender, routing_id);
  function_io->set_extension_info_map(extension_info_map);
  function->set_include_incognito(
      extension_info_map->IsIncognitoEnabled(extension->id()));
  function->Run();
}

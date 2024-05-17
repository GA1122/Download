void ExtensionFunctionDispatcher::DispatchOnIOThread(
    ExtensionInfoMap* extension_info_map,
    void* profile,
    int render_process_id,
    base::WeakPtr<ChromeRenderMessageFilter> ipc_sender,
    int routing_id,
    const ExtensionHostMsg_Request_Params& params) {
  const Extension* extension =
      extension_info_map->extensions().GetByID(params.extension_id);
  Profile* profile_cast = static_cast<Profile*>(profile);
  scoped_refptr<ExtensionFunction> function(
      CreateExtensionFunction(params, extension, render_process_id,
                              extension_info_map->process_map(),
                              g_global_io_data.Get().api.get(),
                              profile,
                              ipc_sender, NULL, routing_id));
  scoped_ptr<ListValue> args(params.arguments.DeepCopy());

  if (!function) {
    LogFailure(extension,
               params.name,
               args.Pass(),
               kAccessDenied,
               profile_cast);
    return;
  }

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

  if (!CheckPermissions(function, extension, params, ipc_sender, routing_id)) {
    LogFailure(extension,
               params.name,
               args.Pass(),
               kAccessDenied,
               profile_cast);
    return;
  }

  ExtensionsQuotaService* quota = extension_info_map->GetQuotaService();
  std::string violation_error = quota->Assess(extension->id(),
                                              function,
                                              &params.arguments,
                                              base::TimeTicks::Now());
  if (violation_error.empty()) {
    LogSuccess(extension,
               params.name,
               args.Pass(),
               profile_cast);
    function->Run();
  } else {
    LogFailure(extension,
               params.name,
               args.Pass(),
               kQuotaExceeded,
               profile_cast);
    function->OnQuotaExceeded(violation_error);
  }
}

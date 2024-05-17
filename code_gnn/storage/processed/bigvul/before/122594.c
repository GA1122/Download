bool ExtensionFunctionDispatcher::CheckPermissions(
    ExtensionFunction* function,
    const Extension* extension,
    const ExtensionHostMsg_Request_Params& params,
    IPC::Sender* ipc_sender,
    int routing_id) {
  if (!function->HasPermission()) {
    LOG(ERROR) << "Extension " << extension->id() << " does not have "
               << "permission to function: " << params.name;
    SendAccessDenied(ipc_sender, routing_id, params.request_id);
    return false;
  }
   return true;
 }

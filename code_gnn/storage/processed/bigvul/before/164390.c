bool CheckSocketPermission(scoped_refptr<Extension> extension,
                           SocketPermissionRequest::OperationType type,
                           const char* host,
                           uint16_t port) {
  SocketPermission::CheckParam param(type, host, port);
  return extension->permissions_data()->CheckAPIPermissionWithParam(
      APIPermission::kSocket, &param);
}

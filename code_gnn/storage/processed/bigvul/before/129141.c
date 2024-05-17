bool RequiresActionForScriptExecution(const std::string& extension_id,
                                      const std::string& host_permissions,
                                      Manifest::Location location) {
  scoped_refptr<const Extension> extension =
      GetExtensionWithHostPermission(extension_id,
                                     host_permissions,
                                     location);
  return extension->permissions_data()->RequiresActionForScriptExecution(
      extension,
      -1,   
       GURL::EmptyGURL());
 }

RenderProcessHostPrivilege GetPrivilegeRequiredByUrl(
    const GURL& url,
    ExtensionService* service) {
  if (!url.is_valid())
    return PRIV_NORMAL;

  if (url.SchemeIs(extensions::kExtensionScheme)) {
    const Extension* extension =
        service->extensions()->GetByID(url.host());
    if (extension && extension->is_storage_isolated())
      return PRIV_ISOLATED;
    if (extension && extension->is_hosted_app())
      return PRIV_HOSTED;

    return PRIV_EXTENSION;
  }

  return PRIV_NORMAL;
}

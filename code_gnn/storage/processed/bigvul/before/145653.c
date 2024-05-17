void ExtensionOptionsGuest::CreateWebContents(
    const base::DictionaryValue& create_params,
    const WebContentsCreatedCallback& callback) {
  std::string extension_id;
  create_params.GetString(extensionoptions::kExtensionId, &extension_id);

  if (!crx_file::id_util::IdIsValid(extension_id)) {
    callback.Run(nullptr);
    return;
  }

  std::string embedder_extension_id = GetOwnerSiteURL().host();
  if (crx_file::id_util::IdIsValid(embedder_extension_id) &&
      extension_id != embedder_extension_id) {
    callback.Run(nullptr);
    return;
  }

  GURL extension_url =
      extensions::Extension::GetBaseURLFromExtensionId(extension_id);
  if (!extension_url.is_valid()) {
    callback.Run(nullptr);
    return;
  }

  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(browser_context());
  const extensions::Extension* extension =
      registry->enabled_extensions().GetByID(extension_id);
  if (!extension) {
    callback.Run(nullptr);
    return;
  }

  options_page_ = extensions::OptionsPageInfo::GetOptionsPage(extension);
  if (!options_page_.is_valid()) {
    callback.Run(nullptr);
    return;
  }

  content::SiteInstance* options_site_instance =
      content::SiteInstance::CreateForURL(browser_context(), extension_url);
  WebContents::CreateParams params(browser_context(), options_site_instance);
  params.guest_delegate = this;
  WebContents* wc = WebContents::Create(params);
  SetViewType(wc, VIEW_TYPE_EXTENSION_GUEST);
  callback.Run(wc);
}

void ExtensionViewGuest::CreateWebContents(
    const base::DictionaryValue& create_params,
    const WebContentsCreatedCallback& callback) {
  std::string extension_id;
  create_params.GetString(extensionview::kAttributeExtension, &extension_id);

  if (!crx_file::id_util::IdIsValid(extension_id) ||
      !IsExtensionIdWhitelisted(extension_id)) {
    callback.Run(nullptr);
    return;
  }

  extension_url_ =
      extensions::Extension::GetBaseURLFromExtensionId(extension_id);

  if (!extension_url_.is_valid()) {
    callback.Run(nullptr);
    return;
  }

  content::SiteInstance* view_site_instance =
      content::SiteInstance::CreateForURL(browser_context(),
                                          extension_url_);

  WebContents::CreateParams params(browser_context(), view_site_instance);
  params.guest_delegate = this;
  callback.Run(WebContents::Create(params));
}

bool WebRequestPermissions::HideRequest(
    const extensions::InfoMap* extension_info_map,
    const net::URLRequest* request) {
  const ResourceRequestInfo* info = ResourceRequestInfo::ForRequest(request);
  if (info) {
    int process_id = info->GetChildID();
    if (extensions::WebViewRendererState::GetInstance()->IsGuest(process_id))
      return false;

    if (extension_info_map &&
        extension_info_map->process_map().Contains(extensions::kWebStoreAppId,
                                                   process_id)) {
      return true;
    }
  }

  const GURL& url = request->url();
  return IsSensitiveURL(url) || !HasWebRequestScheme(url);
}

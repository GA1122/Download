ExtensionResource Extension::GetContentPackSiteList() const {
  if (!is_content_pack())
    return ExtensionResource();

  return GetResource(content_pack_site_list_);
}

bool Extension::UpdatesFromGallery() const {
  return extension_urls::IsWebstoreUpdateUrl(ManifestURL::GetUpdateURL(this));
}

FilePath NaClProcessHost::GetManifestPath() {
  const extensions::Extension* extension = extension_info_map_->extensions()
      .GetExtensionOrAppByURL(ExtensionURLInfo(manifest_url_));
  if (extension != NULL && manifest_url_.SchemeIs(chrome::kExtensionScheme)) {
    std::string path = manifest_url_.path();
    TrimString(path, "/", &path);   
    return extension->path().AppendASCII(path);
  }
  return FilePath();
}

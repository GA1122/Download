void Extension::DecodeIcon(const Extension* extension,
                           int preferred_icon_size,
                           ExtensionIconSet::MatchType match_type,
                           scoped_ptr<SkBitmap>* result) {
  std::string path = extension->icons().Get(preferred_icon_size, match_type);
  int size = extension->icons().GetIconSizeFromPath(path);
  ExtensionResource icon_resource = extension->GetResource(path);
  DecodeIconFromPath(icon_resource.GetFilePath(), size, result);
}

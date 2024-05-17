void Extension::DecodeIcon(const Extension* extension,
                           int icon_size,
                           scoped_ptr<SkBitmap>* result) {
  DecodeIcon(extension, icon_size, ExtensionIconSet::MATCH_EXACTLY, result);
}

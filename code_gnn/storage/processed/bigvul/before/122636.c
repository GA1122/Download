GURL Extension::GetBackgroundURL() const {
  if (background_scripts_.empty())
    return background_url_;
  return GetResourceURL(extension_filenames::kGeneratedBackgroundPageFilename);
}

void FeatureInfo::EnableCHROMIUMTextureStorageImage() {
  if (!feature_flags_.chromium_texture_storage_image) {
    feature_flags_.chromium_texture_storage_image = true;
    AddExtensionString("GL_CHROMIUM_texture_storage_image");
  }
}

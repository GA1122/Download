void AppResult::OnExtensionIconImageChanged(extensions::IconImage* image) {
  DCHECK_EQ(icon_.get(), image);
  UpdateIcon();
}

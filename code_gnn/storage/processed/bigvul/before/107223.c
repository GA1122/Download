int AutomationProvider::AddExtension(const Extension* extension) {
  DCHECK(extension);
  return extension_tracker_->Add(extension);
}

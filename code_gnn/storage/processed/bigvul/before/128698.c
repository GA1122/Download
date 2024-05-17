std::string TemplateURL::GetExtensionId() const {
  DCHECK(extension_info_);
  return extension_info_->extension_id;
}

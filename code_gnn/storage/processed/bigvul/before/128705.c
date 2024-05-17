void TemplateURLRef::HandleReplacement(const std::string& name,
                                       const std::string& value,
                                       const Replacement& replacement,
                                       std::string* url) const {
  size_t pos = replacement.index;
  if (replacement.is_post_param) {
    DCHECK_LT(pos, post_params_.size());
    DCHECK(!post_params_[pos].name.empty());
    post_params_[pos].value = value;
  } else {
    url->insert(pos, name.empty() ? value : (name + "=" + value + "&"));
  }
}

const TemplateURL* ZeroSuggestProvider::GetTemplateURL(bool is_keyword) const {
  DCHECK(!is_keyword);
  return client()->GetTemplateURLService()->GetDefaultSearchProvider();
}

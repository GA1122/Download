TemplateURLRef::TemplateURLRef(const TemplateURL* owner, size_t index_in_owner)
    : owner_(owner),
      type_(INDEXED),
      index_in_owner_(index_in_owner),
      parsed_(false),
      valid_(false),
      supports_replacements_(false),
      search_term_position_in_path_(std::string::npos),
      search_term_key_location_(url::Parsed::QUERY),
      prepopulated_(false) {
  DCHECK(owner_);
  DCHECK_LT(index_in_owner_, owner_->alternate_urls().size());
}

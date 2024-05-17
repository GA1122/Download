TemplateURLRef::TemplateURLRef(const TemplateURL* owner, Type type)
    : owner_(owner),
      type_(type),
      index_in_owner_(0),
      parsed_(false),
      valid_(false),
      supports_replacements_(false),
      search_term_position_in_path_(std::string::npos),
      search_term_key_location_(url::Parsed::QUERY),
      prepopulated_(false) {
  DCHECK(owner_);
  DCHECK_NE(INDEXED, type_);
}

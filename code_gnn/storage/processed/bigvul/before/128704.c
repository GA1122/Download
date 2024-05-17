std::string TemplateURLRef::GetURL() const {
  switch (type_) {
    case SEARCH:            return owner_->url();
    case SUGGEST:           return owner_->suggestions_url();
    case INSTANT:           return owner_->instant_url();
    case IMAGE:             return owner_->image_url();
    case NEW_TAB:           return owner_->new_tab_url();
    case CONTEXTUAL_SEARCH: return owner_->contextual_search_url();
    case INDEXED:           return owner_->alternate_urls()[index_in_owner_];
    default:       NOTREACHED(); return std::string();   
  }
}

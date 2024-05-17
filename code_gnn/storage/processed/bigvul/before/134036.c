scoped_ptr<SearchResult> AppResult::Duplicate() const {
  scoped_ptr<SearchResult> copy(
      new AppResult(profile_, app_id_, controller_,
                    display_type() == DISPLAY_RECOMMENDATION));
  copy->set_title(title());
  copy->set_title_tags(title_tags());
  copy->set_relevance(relevance());

  return copy.Pass();
}

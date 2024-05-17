void PDFiumEngine::StopFind() {
  SelectionChangeInvalidator selection_invalidator(this);
  selection_.clear();
  selecting_ = false;

  find_results_.clear();
  next_page_to_search_ = -1;
  last_page_to_search_ = -1;
  last_character_index_to_search_ = -1;
  current_find_index_.reset();
  current_find_text_.clear();

  UpdateTickMarks();
  find_factory_.CancelAll();
}

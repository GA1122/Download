void PDFiumEngine::AddFindResult(const PDFiumRange& result) {
  size_t result_index;
  int page_index = result.page_index();
  int char_index = result.char_index();
  for (result_index = 0; result_index < find_results_.size(); ++result_index) {
    if (find_results_[result_index].page_index() > page_index ||
        (find_results_[result_index].page_index() == page_index &&
         find_results_[result_index].char_index() > char_index)) {
      break;
    }
  }
  find_results_.insert(find_results_.begin() + result_index, result);
  UpdateTickMarks();

  if (current_find_index_.valid()) {
    if (result_index <= current_find_index_.GetIndex()) {
      size_t find_index = current_find_index_.IncrementIndex();
      DCHECK_LT(find_index, find_results_.size());
      client_->NotifySelectedFindResultChanged(current_find_index_.GetIndex());
    }
  } else if (!resume_find_index_.valid()) {
    SelectFindResult(true);
  }
  client_->NotifyNumberOfFindResultsChanged(find_results_.size(), false);
}

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
  client_->NotifyNumberOfFindResultsChanged(find_results_.size(), false);
}

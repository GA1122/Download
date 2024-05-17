void WebPluginDelegatePepper::NumberOfFindResultsChanged(int total,
                                                         bool final_result) {
  DCHECK(find_identifier_ != -1);

  if (total == 0) {
    render_view_->ReportNoFindInPageResults(find_identifier_);
  } else {
    render_view_->reportFindInPageMatchCount(
        find_identifier_, total, final_result);
  }
}

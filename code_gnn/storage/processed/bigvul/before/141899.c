AutofillPopupSuggestionView* AutofillPopupSuggestionView::Create(
    AutofillPopupViewNativeViews* popup_view,
    int line_number,
    int frontend_id) {
  AutofillPopupSuggestionView* result =
      new AutofillPopupSuggestionView(popup_view, line_number, frontend_id);
  result->Init();
  return result;
}

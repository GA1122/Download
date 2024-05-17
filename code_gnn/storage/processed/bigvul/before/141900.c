AutofillPopupFooterView* AutofillPopupFooterView::Create(
    AutofillPopupViewNativeViews* popup_view,
    int line_number,
    int frontend_id) {
  AutofillPopupFooterView* result =
      new AutofillPopupFooterView(popup_view, line_number, frontend_id);
  result->Init();
  return result;
}

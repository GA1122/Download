AutofillPopupFooterView::AutofillPopupFooterView(
    AutofillPopupViewNativeViews* popup_view,
    int line_number,
    int frontend_id)
    : AutofillPopupItemView(popup_view,
                            line_number,
                            frontend_id,
                            AutofillPopupBaseView::GetCornerRadius()) {
  SetFocusBehavior(FocusBehavior::ALWAYS);
}

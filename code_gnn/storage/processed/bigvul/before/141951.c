AutofillPopupViewViews::AutofillPopupViewViews(
    AutofillPopupController* controller,
    views::Widget* parent_widget)
    : AutofillPopupBaseView(controller, parent_widget),
      controller_(controller) {
  CreateChildViews();
  SetFocusBehavior(FocusBehavior::ALWAYS);
}

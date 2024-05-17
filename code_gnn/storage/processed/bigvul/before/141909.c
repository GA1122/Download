void AutofillPopupWarningView::CreateContent() {
  AutofillPopupController* controller = popup_view_->controller();

  int horizontal_margin = GetHorizontalMargin();
  int vertical_margin = AutofillPopupBaseView::GetCornerRadius();

  SetLayoutManager(std::make_unique<views::FillLayout>());
  SetBorder(views::CreateEmptyBorder(
      gfx::Insets(vertical_margin, horizontal_margin)));

  views::Label* text_label = CreateLabelWithColorReadabilityDisabled(
      controller->GetElidedValueAt(line_number_),
      ChromeTextContext::CONTEXT_BODY_TEXT_LARGE, ChromeTextStyle::STYLE_RED);
  text_label->SetEnabledColor(popup_view_->GetWarningColor());
  text_label->SetMultiLine(true);
  int max_width =
      std::min(kAutofillPopupMaxWidth,
               PopupViewCommon().CalculateMaxWidth(
                   gfx::ToEnclosingRect(controller->element_bounds()),
                   controller->container_view()));
  max_width -= 2 * horizontal_margin;
  text_label->SetMaximumWidth(max_width);
  text_label->SetHorizontalAlignment(gfx::HorizontalAlignment::ALIGN_LEFT);

  AddChildView(text_label);
}

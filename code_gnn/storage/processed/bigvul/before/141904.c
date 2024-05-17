std::unique_ptr<views::Background> AutofillPopupFooterView::CreateBackground() {
  return views::CreateSolidBackground(
      is_selected_ ? popup_view_->GetSelectedBackgroundColor()
                   : popup_view_->GetFooterBackgroundColor());
}

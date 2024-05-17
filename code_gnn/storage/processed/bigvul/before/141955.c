std::unique_ptr<views::View> AutofillPopupViewViews::CreateWrapperView() {
  auto wrapper_view = std::make_unique<views::ScrollView>();
  scroll_view_ = wrapper_view.get();
  scroll_view_->set_hide_horizontal_scrollbar(true);
  scroll_view_->SetContents(this);
  return wrapper_view;
}

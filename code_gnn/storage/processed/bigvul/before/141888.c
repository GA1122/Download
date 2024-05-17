void AutofillPopupItemView::AddIcon(gfx::ImageSkia icon) {
  auto* image_view = new views::ImageView();
  image_view->SetImage(icon);
  AddChildView(image_view);
}

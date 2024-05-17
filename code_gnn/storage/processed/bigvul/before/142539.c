ShelfWidget::DelegateView::DelegateView(ShelfWidget* shelf_widget)
    : shelf_widget_(shelf_widget),
      focus_cycler_(nullptr),
      opaque_background_(ui::LAYER_SOLID_COLOR) {
  DCHECK(shelf_widget_);
  set_owned_by_client();   

  SetLayoutManager(std::make_unique<views::FillLayout>());
  set_allow_deactivate_on_esc(true);

  UpdateOpaqueBackground();
}

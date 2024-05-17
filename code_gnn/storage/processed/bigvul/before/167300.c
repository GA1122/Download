void ScrollableShelfView::Init() {
  shelf_view_->Init();

  SetPaintToLayer();
  layer()->SetFillsBoundsOpaquely(false);

  left_arrow_ = AddChildView(std::make_unique<ScrollArrowView>(
      ScrollArrowView::kLeft, GetShelf()->IsHorizontalAlignment(), GetShelf(),
      this));

  right_arrow_ = AddChildView(std::make_unique<ScrollArrowView>(
      ScrollArrowView::kRight, GetShelf()->IsHorizontalAlignment(), GetShelf(),
      this));

  shelf_container_view_ =
      AddChildView(std::make_unique<ShelfContainerView>(shelf_view_));
  shelf_container_view_->Initialize();

  gradient_layer_delegate_ = std::make_unique<GradientLayerDelegate>();
  layer()->SetMaskLayer(gradient_layer_delegate_->layer());

  focus_search_ = std::make_unique<ScrollableShelfFocusSearch>(this);

  GetShelf()->tooltip()->set_shelf_tooltip_delegate(shelf_view_);
}

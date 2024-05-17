ShelfWidget::ShelfWidget(aura::Window* shelf_container, Shelf* shelf)
    : shelf_(shelf),
      background_animator_(SHELF_BACKGROUND_DEFAULT,
                           shelf_,
                           Shell::Get()->wallpaper_controller()),
      shelf_layout_manager_(new ShelfLayoutManager(this, shelf)),
      delegate_view_(new DelegateView(this)),
      shelf_view_(new ShelfView(Shell::Get()->shelf_model(), shelf_, this)),
      login_shelf_view_(
          new LoginShelfView(RootWindowController::ForWindow(shelf_container)
                                 ->lock_screen_action_background_controller())),
      scoped_session_observer_(this) {
  DCHECK(shelf_container);
  DCHECK(shelf_);

  views::Widget::InitParams params(
      views::Widget::InitParams::TYPE_WINDOW_FRAMELESS);
  params.name = "ShelfWidget";
  params.layer_type = ui::LAYER_NOT_DRAWN;
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.delegate = delegate_view_;
  params.parent = shelf_container;

  Init(params);

  set_focus_on_creation(false);
  SetContentsView(delegate_view_);
  delegate_view_->SetParentLayer(GetLayer());

  shelf_view_->Init();
  GetContentsView()->AddChildView(shelf_view_);
  GetContentsView()->AddChildView(login_shelf_view_);

  shelf_layout_manager_->AddObserver(this);
  shelf_container->SetLayoutManager(shelf_layout_manager_);
  background_animator_.PaintBackground(
      shelf_layout_manager_->GetShelfBackgroundType(),
      AnimationChangeType::IMMEDIATE);

  views::Widget::AddObserver(this);

  background_animator_.AddObserver(delegate_view_);
  shelf_->AddObserver(this);
}

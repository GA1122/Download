Launcher::Launcher(aura::Window* window_container,
                   internal::ShelfLayoutManager* shelf_layout_manager)
    : widget_(NULL),
      window_container_(window_container),
      delegate_view_(NULL),
      launcher_view_(NULL),
      alignment_(SHELF_ALIGNMENT_BOTTOM),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          background_animator_(this, 0, kBackgroundAlpha)) {
  model_.reset(new LauncherModel);
  if (Shell::GetInstance()->delegate()) {
    delegate_.reset(
        Shell::GetInstance()->delegate()->CreateLauncherDelegate(model_.get()));
  }

  widget_.reset(new views::Widget);
  views::Widget::InitParams params(
      views::Widget::InitParams::TYPE_WINDOW_FRAMELESS);
  params.layer_type = ui::LAYER_SOLID_COLOR;
  params.transparent = true;
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.parent = Shell::GetContainer(
      window_container_->GetRootWindow(),
      ash::internal::kShellWindowId_LauncherContainer);
  launcher_view_ = new internal::LauncherView(
      model_.get(), delegate_.get(), shelf_layout_manager);
  launcher_view_->Init();
  delegate_view_ = new DelegateView(this);
  delegate_view_->AddChildView(launcher_view_);
  params.delegate = delegate_view_;
  widget_->Init(params);
  widget_->GetNativeWindow()->SetName("LauncherWindow");
  gfx::Size pref =
      static_cast<views::View*>(launcher_view_)->GetPreferredSize();
  widget_->SetBounds(gfx::Rect(pref));
  widget_->set_focus_on_creation(false);
  widget_->SetContentsView(delegate_view_);
  widget_->Show();
  widget_->GetNativeView()->SetName("LauncherView");
}

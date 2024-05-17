  aura::Window* CreateWindowWithWidget(const gfx::Rect& bounds) {
    views::Widget* widget = new views::Widget();
    views::Widget::InitParams params;
    params.context = CurrentContext();
    widget->Init(params);
    widget->Show();
    aura::Window* window = widget->GetNativeWindow();
    window->SetBounds(bounds);
    window->SetProperty(aura::client::kShowStateKey, ui::SHOW_STATE_NORMAL);

    return window;
  }

  aura::Window* OpenTestWindow(aura::Window* parent) {
    views::Widget* widget =
        views::Widget::CreateWindowWithParent(this, parent);
    widget->Show();
    return widget->GetNativeView();
  }

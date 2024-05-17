  aura::Window* OpenTestWindowWithContext(aura::Window* context) {
    views::Widget* widget =
        views::Widget::CreateWindowWithContext(this, context);
    widget->Show();
    return widget->GetNativeView();
  }

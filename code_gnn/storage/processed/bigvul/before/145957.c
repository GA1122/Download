  aura::Window* OpenTestWindowWithParent(aura::Window* parent) {
    DCHECK(parent);
    views::Widget* widget = views::Widget::CreateWindowWithParent(this, parent);
    widget->Show();
    return widget->GetNativeView();
  }

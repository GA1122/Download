   aura::Window* OpenToplevelTestWindow(bool modal) {
    views::Widget* widget = views::Widget::CreateWindowWithContext(
        new TestWindow(modal), CurrentContext());
    widget->Show();
    return widget->GetNativeView();
  }

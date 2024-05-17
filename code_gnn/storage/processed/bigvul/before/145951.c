  static void CloseTestWindow(aura::Window* window) {
    views::Widget::GetWidgetForNativeWindow(window)->Close();
  }

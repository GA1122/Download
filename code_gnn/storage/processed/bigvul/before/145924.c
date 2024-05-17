  bool IsPanelCalloutVisible(aura::Window* panel) {
    views::Widget* widget = GetCalloutWidgetForPanel(panel);
    return widget->IsVisible();
  }

void BrowserView::ZoomChangedForActiveTab(bool can_show_bubble) {
  const AppMenuButton* app_menu_button =
      toolbar_button_provider()->GetAppMenuButton();
  bool app_menu_showing = app_menu_button && app_menu_button->IsMenuShowing();
  toolbar_button_provider()
      ->GetPageActionIconContainerView()
      ->ZoomChangedForActiveTab(can_show_bubble && !app_menu_showing);
}

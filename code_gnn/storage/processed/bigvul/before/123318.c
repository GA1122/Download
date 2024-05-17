void RenderWidgetHostViewGtk::OnAccessibilityNotifications(
    const std::vector<AccessibilityHostMsg_NotificationParams>& params) {
  if (!browser_accessibility_manager_.get()) {
    GtkWidget* parent = gtk_widget_get_parent(view_.get());
    browser_accessibility_manager_.reset(
        BrowserAccessibilityManager::CreateEmptyDocument(
            parent,
            static_cast<AccessibilityNodeData::State>(0),
            this));
  }
  browser_accessibility_manager_->OnAccessibilityNotifications(params);
}

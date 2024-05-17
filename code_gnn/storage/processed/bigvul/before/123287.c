AtkObject* RenderWidgetHostViewGtk::GetAccessible() {
  if (!browser_accessibility_manager_.get()) {
    GtkWidget* parent = gtk_widget_get_parent(view_.get());
    browser_accessibility_manager_.reset(
        BrowserAccessibilityManager::CreateEmptyDocument(
            parent,
            static_cast<AccessibilityNodeData::State>(0),
            this));
  }
  BrowserAccessibilityGtk* root =
      browser_accessibility_manager_->GetRoot()->ToBrowserAccessibilityGtk();

  atk_object_set_role(root->GetAtkObject(), ATK_ROLE_HTML_CONTAINER);
  return root->GetAtkObject();
}

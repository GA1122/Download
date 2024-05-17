 void WebsiteSettingsPopupView::OnPermissionChanged(
    const WebsiteSettingsUI::PermissionInfo& permission) {
  presenter_->OnSitePermissionChanged(permission.type, permission.setting);
}

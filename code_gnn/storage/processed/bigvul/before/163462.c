void OmniboxViewViews::UpdateSecurityLevel() {
  security_level_ = controller()->GetToolbarModel()->GetSecurityLevel(false);
}

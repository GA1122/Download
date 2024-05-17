void OmniboxViewViews::Update() {
  const ToolbarModel::SecurityLevel old_security_level = security_level_;
  security_level_ = controller()->GetToolbarModel()->GetSecurityLevel(false);
  if (model()->UpdatePermanentText()) {
    controller()->GetToolbarModel()->set_url_replacement_enabled(true);
    model()->UpdatePermanentText();

    const gfx::Range range(GetSelectedRange());
    const bool was_select_all = (range.length() == text().length());

    RevertAll();

    if (was_select_all && model()->has_focus())
      SelectAll(range.is_reversed());
  } else if (old_security_level != security_level_) {
    EmphasizeURLComponents();
  }
}

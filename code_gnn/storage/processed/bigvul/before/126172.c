Browser::CreateParams Browser::CreateParams::CreateForApp(
    Type type,
    const std::string& app_name,
    const gfx::Rect& window_bounds,
    Profile* profile) {
  DCHECK(type != TYPE_TABBED);
  DCHECK(!app_name.empty());

  CreateParams params(type, profile);
  params.app_name = app_name;
  params.app_type = APP_TYPE_CHILD;
  params.initial_bounds = window_bounds;

  return params;
}

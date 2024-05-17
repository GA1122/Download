Browser* Browser::CreateForApp(Type type,
                               const std::string& app_name,
                               const gfx::Rect& window_bounds,
                               Profile* profile) {
  DCHECK(type != TYPE_TABBED);
  DCHECK(!app_name.empty());

  RegisterAppPrefs(app_name, profile);

#if !defined(OS_CHROMEOS)
  if (type == TYPE_PANEL &&
      !CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnablePanels)) {
    type = TYPE_POPUP;
  }
#endif

  CreateParams params(type, profile);
  params.app_name = app_name;
  if (!window_bounds.IsEmpty())
    params.initial_bounds = window_bounds;

  return CreateWithParams(params);
}

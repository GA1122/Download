void TabHelper::CreateApplicationShortcuts() {
  DCHECK(CanCreateApplicationShortcuts());
  if (pending_web_app_action_ != NONE)
    return;

  GetApplicationInfo(CREATE_SHORTCUT);
}

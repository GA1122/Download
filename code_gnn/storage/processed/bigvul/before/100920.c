void TaskManagerHandler::OpenAboutMemory(const ListValue* indexes) {
  RenderViewHost* rvh = web_ui_->tab_contents()->GetRenderViewHost();
  if (rvh && rvh->delegate()) {
    WebPreferences webkit_prefs = rvh->delegate()->GetWebkitPrefs();
    webkit_prefs.allow_scripts_to_close_windows = true;
    rvh->UpdateWebkitPreferences(webkit_prefs);
  } else {
    DCHECK(false);
  }

  task_manager_->OpenAboutMemory();
}

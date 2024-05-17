void CloseBalloon(const std::string id) {
  g_browser_process->notification_ui_manager()->CancelById(id);
}

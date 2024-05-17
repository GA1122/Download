FPSObserver::FPSObserver(
    AutomationProvider* automation,
    IPC::Message* reply_message,
    base::ProcessId renderer_id,
    int routing_id)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      renderer_id_(renderer_id),
      routing_id_(routing_id) {
  registrar_.Add(
      this,
      chrome::NOTIFICATION_RENDERER_FPS_COMPUTED,
      content::NotificationService::AllSources());
}

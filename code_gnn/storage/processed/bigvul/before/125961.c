InputEventAckNotificationObserver::InputEventAckNotificationObserver(
    AutomationProvider* automation,
    IPC::Message* reply_message,
    int event_type,
    int count)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      event_type_(event_type),
      count_(count) {
  DCHECK(1 <= count);
  registrar_.Add(
      this,
      content::NOTIFICATION_RENDER_WIDGET_HOST_DID_RECEIVE_INPUT_EVENT_ACK,
      content::NotificationService::AllSources());
  registrar_.Add(
      this,
      chrome::NOTIFICATION_APP_MODAL_DIALOG_SHOWN,
      content::NotificationService::AllSources());
}

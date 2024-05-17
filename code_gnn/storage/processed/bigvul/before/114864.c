void TestingAutomationProvider::CloseNotification(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  int index;
  if (!args->GetInteger("index", &index)) {
    AutomationJSONReply(this, reply_message)
        .SendError("'index' missing or invalid.");
    return;
  }
  NotificationUIManager* manager = g_browser_process->notification_ui_manager();
  BalloonCollection* collection = manager->balloon_collection();
  const BalloonCollection::Balloons& balloons = collection->GetActiveBalloons();
  int balloon_count = static_cast<int>(balloons.size());
  if (index < 0 || index >= balloon_count) {
    AutomationJSONReply(this, reply_message)
        .SendError(StringPrintf("No notification at index %d", index));
    return;
  }
  std::vector<const Notification*> queued_notes;
  manager->GetQueuedNotificationsForTesting(&queued_notes);
  if (queued_notes.empty()) {
    new OnNotificationBalloonCountObserver(
        this, reply_message, balloon_count - 1);
  } else {
    new NewNotificationBalloonObserver(this, reply_message);
  }
  manager->CancelById(balloons[index]->notification().notification_id());
}

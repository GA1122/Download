void InputEventAckNotificationObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (type == chrome::NOTIFICATION_APP_MODAL_DIALOG_SHOWN) {
    AutomationJSONReply(automation_,
                        reply_message_.release()).SendSuccess(NULL);
    delete this;
    return;
  }

  content::Details<int> request_details(details);
  if (event_type_ == *request_details.ptr()) {
    if (--count_ == 0 && automation_) {
      AutomationJSONReply(automation_,
                          reply_message_.release()).SendSuccess(NULL);
      delete this;
    }
  } else {
    LOG(WARNING) << "Ignoring unexpected event type: "
                 << *request_details.ptr() << " (expected: " << event_type_
                 << ")";
  }
}

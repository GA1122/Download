base::DictionaryValue* GetAllNotificationsObserver::NotificationToJson(
    const Notification* note) {
  DictionaryValue* dict = new base::DictionaryValue();
  dict->SetString("content_url", note->content_url().spec());
  dict->SetString("origin_url", note->origin_url().spec());
  dict->SetString("display_source", note->display_source());
  dict->SetString("id", note->notification_id());
  return dict;
}

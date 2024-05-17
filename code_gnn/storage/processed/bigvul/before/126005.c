void FPSObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK(type == chrome::NOTIFICATION_RENDERER_FPS_COMPUTED);

  base::ProcessId updated_renderer_id =
      *(content::Source<base::ProcessId>(source).ptr());
  if (renderer_id_ != updated_renderer_id)
    return;

  ChromeRenderMessageFilter::FPSDetails* fps_details =
      content::Details<ChromeRenderMessageFilter::FPSDetails>(details).ptr();
  if (routing_id_ != fps_details->routing_id())
    return;

  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  return_value->SetInteger("renderer_id", updated_renderer_id);
  return_value->SetInteger("routing_id", fps_details->routing_id());
  return_value->SetDouble("fps", fps_details->fps());
  if (automation_) {
    AutomationJSONReply(automation_, reply_message_.release())
        .SendSuccess(return_value.get());
  }
  delete this;
}

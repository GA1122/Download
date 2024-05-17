void V8HeapStatsObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK(type == chrome::NOTIFICATION_RENDERER_V8_HEAP_STATS_COMPUTED);

  base::ProcessId updated_renderer_id =
      *(content::Source<base::ProcessId>(source).ptr());
  if (renderer_id_ != updated_renderer_id)
    return;

  ChromeRenderMessageFilter::V8HeapStatsDetails* v8_heap_details =
      content::Details<ChromeRenderMessageFilter::V8HeapStatsDetails>(details)
          .ptr();
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  return_value->SetInteger("renderer_id", updated_renderer_id);
  return_value->SetInteger("v8_memory_allocated",
                           v8_heap_details->v8_memory_allocated());
  return_value->SetInteger("v8_memory_used",
                           v8_heap_details->v8_memory_used());

  if (automation_) {
    AutomationJSONReply(automation_, reply_message_.release())
        .SendSuccess(return_value.get());
  }
  delete this;
}

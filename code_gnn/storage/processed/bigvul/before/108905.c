void RenderViewImpl::LaunchAndroidContentIntent(const GURL& intent,
                                                size_t request_id) {
  if (request_id != expected_content_intent_id_)
      return;

  scheduleComposite();

  if (!intent.is_empty())
    Send(new ViewHostMsg_StartContentIntent(routing_id_, intent));
}

void RenderViewImpl::scheduleContentIntent(const WebURL& intent) {
  MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&RenderViewImpl::LaunchAndroidContentIntent, AsWeakPtr(),
          intent, expected_content_intent_id_),
      base::TimeDelta::FromMilliseconds(kContentIntentDelayMilliseconds));
}

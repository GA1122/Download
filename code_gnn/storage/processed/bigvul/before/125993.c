void AutomationMouseEventProcessor::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  InvokeCallback(automation::Error(automation::kBlockedByModalDialog));
}

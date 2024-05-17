void RenderViewHostImpl::OnDomOperationResponse(
    const std::string& json_string, int automation_id) {
  DomOperationNotificationDetails details(json_string, automation_id);
  NotificationService::current()->Notify(
      NOTIFICATION_DOM_OPERATION_RESPONSE,
      Source<RenderViewHost>(this),
      Details<DomOperationNotificationDetails>(&details));
}

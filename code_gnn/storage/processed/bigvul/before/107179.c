void AutoFillManager::OnDidShowAutoFillSuggestions() {
  NotificationService::current()->Notify(
      NotificationType::AUTOFILL_DID_SHOW_SUGGESTIONS,
      Source<RenderViewHost>(tab_contents_->render_view_host()),
      NotificationService::NoDetails());
}

void AutofillManager::OnDidShowAutofillSuggestions() {
  NotificationService::current()->Notify(
      NotificationType::AUTOFILL_DID_SHOW_SUGGESTIONS,
      Source<RenderViewHost>(tab_contents()->render_view_host()),
      NotificationService::NoDetails());
}

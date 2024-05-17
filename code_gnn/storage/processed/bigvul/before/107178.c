void AutoFillManager::OnDidFillAutoFillFormData() {
  NotificationService::current()->Notify(
      NotificationType::AUTOFILL_DID_FILL_FORM_DATA,
      Source<RenderViewHost>(tab_contents_->render_view_host()),
      NotificationService::NoDetails());
}

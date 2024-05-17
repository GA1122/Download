DateTimeChooser* ChromeClientImpl::OpenDateTimeChooser(
    DateTimeChooserClient* picker_client,
    const DateTimeChooserParameters& parameters) {
  if (picker_client->OwnerElement()
          .GetDocument()
          .GetSettings()
          ->GetPagePopupsSuppressed())
    return nullptr;

  NotifyPopupOpeningObservers();
  if (RuntimeEnabledFeatures::InputMultipleFieldsUIEnabled())
    return DateTimeChooserImpl::Create(this, picker_client, parameters);
  return ExternalDateTimeChooser::Create(this, web_view_->Client(),
                                         picker_client, parameters);
}

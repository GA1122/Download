ColorChooser* ChromeClientImpl::OpenColorChooser(
    LocalFrame* frame,
    ColorChooserClient* chooser_client,
    const Color&) {
  NotifyPopupOpeningObservers();
  ColorChooserUIController* controller = nullptr;

  if (frame->GetDocument()->GetSettings()->GetPagePopupsSuppressed())
    return nullptr;

  if (RuntimeEnabledFeatures::PagePopupEnabled()) {
    controller =
        ColorChooserPopupUIController::Create(frame, this, chooser_client);
  } else {
    controller = ColorChooserUIController::Create(frame, chooser_client);
  }
  controller->OpenUI();
  return controller;
}

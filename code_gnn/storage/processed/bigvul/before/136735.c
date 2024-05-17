void LocalDOMWindow::AcceptLanguagesChanged() {
  if (navigator_)
    navigator_->SetLanguagesChanged();

  DispatchEvent(Event::Create(EventTypeNames::languagechange));
}

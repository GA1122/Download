void MediaControlDownloadButtonElement::defaultEventHandler(Event* event) {
  const KURL& url = mediaElement().currentSrc();
  if (event->type() == EventTypeNames::click &&
      !(url.isNull() || url.isEmpty())) {
    Platform::current()->recordAction(
        UserMetricsAction("Media.Controls.Download"));
    if (!m_anchor) {
      HTMLAnchorElement* anchor = HTMLAnchorElement::create(document());
      anchor->setAttribute(HTMLNames::downloadAttr, "");
      m_anchor = anchor;
    }
    m_anchor->setURL(url);
    m_anchor->dispatchSimulatedClick(event);
  }
  MediaControlInputElement::defaultEventHandler(event);
}

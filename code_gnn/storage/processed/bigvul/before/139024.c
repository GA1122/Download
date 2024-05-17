static ImageEventSender& errorEventSender() {
  DEFINE_STATIC_LOCAL(ImageEventSender, sender,
                      (ImageEventSender::create(EventTypeNames::error)));
  return sender;
}

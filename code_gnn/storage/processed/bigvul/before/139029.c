static ImageEventSender& loadEventSender() {
  DEFINE_STATIC_LOCAL(ImageEventSender, sender,
                      (ImageEventSender::create(EventTypeNames::load)));
  return sender;
}

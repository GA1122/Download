SystemClipboard& SystemClipboard::GetInstance() {
  DEFINE_STATIC_LOCAL(SystemClipboard, clipboard, ());
  return clipboard;
}

int ExtensionTabUtil::GetWindowId(const Browser* browser) {
  return browser->session_id().id();
}

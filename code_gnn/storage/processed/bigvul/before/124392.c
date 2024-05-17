int ExtensionTabUtil::GetWindowIdOfTab(const WebContents* web_contents) {
  return SessionID::IdForWindowContainingTab(web_contents);
}

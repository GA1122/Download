int ExtensionTabUtil::GetTabId(const WebContents* web_contents) {
  return SessionID::IdForTab(web_contents);
}

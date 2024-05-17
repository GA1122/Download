bool ExtensionDevToolsClientHost::MatchesContentsAndExtensionId(
    WebContents* web_contents,
    const std::string& extension_id) {
  return web_contents == web_contents_ && extension_id_ == extension_id;
}

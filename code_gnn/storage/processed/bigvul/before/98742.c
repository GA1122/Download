bool WebPluginDelegatePepper::SetCursor(NPCursorType type) {
  cursor_.reset(new WebCursorInfo(static_cast<WebCursorInfo::Type>(type)));
  return true;
}

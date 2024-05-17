int AeroPeekManager::GetTabID(TabContents* contents) const {
  if (!contents)
    return -1;
  return contents->controller().session_id().id();
}

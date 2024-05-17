TabContents* AeroPeekManager::GetTabContents(int tab_id) const {
  for (TabContentsIterator iterator; !iterator.done(); ++iterator) {
    TabContents* target_contents = *iterator;
    if (target_contents->controller().session_id().id() == tab_id)
      return target_contents;
  }
  return NULL;
}

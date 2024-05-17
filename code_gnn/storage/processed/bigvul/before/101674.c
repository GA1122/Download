TabContents* Browser::GetTabContentsAt(int index) const {
  TabContentsWrapper* wrapper = tabstrip_model()->GetTabContentsAt(index);
  if (wrapper)
    return wrapper->tab_contents();
  return NULL;
}

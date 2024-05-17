TabContents* Browser::GetSelectedTabContents() const {
  TabContentsWrapper* wrapper = GetSelectedTabContentsWrapper();
  if (wrapper)
    return wrapper->tab_contents();
  return NULL;
}

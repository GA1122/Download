Browser* Browser::CreateForPopup(Type type,
                                 Profile* profile,
                                 TabContents* new_contents,
                                 const gfx::Rect& initial_bounds) {
  DCHECK(type & TYPE_POPUP);
  Browser* browser = new Browser(type, profile);
  browser->set_override_bounds(initial_bounds);
  browser->CreateBrowserWindow();
  TabContentsWrapper* wrapper = new TabContentsWrapper(new_contents);
  browser->tabstrip_model()->AppendTabContents(wrapper, true);
  return browser;
}

bool Browser::CanReloadContents(content::WebContents* web_contents) const {
  return chrome::CanReload(this);
}

void GlobalConfirmInfoBar::TabInsertedAt(TabStripModel* tab_strip_model,
                                         content::WebContents* web_contents,
                                         int index,
                                         bool foreground) {
  MaybeAddInfoBar(web_contents);
}

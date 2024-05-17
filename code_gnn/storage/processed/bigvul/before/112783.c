TabContents* PrintPreviewHandler::preview_tab_contents() const {
  return TabContents::FromWebContents(preview_web_contents());
}

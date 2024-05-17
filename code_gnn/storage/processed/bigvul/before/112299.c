void ShellWindow::ToggleFullscreenModeForTab(content::WebContents* source,
                                             bool enter_fullscreen) {
  DCHECK(source == web_contents_);
  SetFullscreen(enter_fullscreen);
}

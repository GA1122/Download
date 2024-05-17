void ShellWindow::AddNewContents(WebContents* source,
                                 WebContents* new_contents,
                                 WindowOpenDisposition disposition,
                                 const gfx::Rect& initial_pos,
                                 bool user_gesture) {
  DCHECK(source == web_contents_);
  DCHECK(Profile::FromBrowserContext(new_contents->GetBrowserContext()) ==
      profile_);
  Browser* browser = browser::FindOrCreateTabbedBrowser(profile_);
  disposition =
      disposition == NEW_BACKGROUND_TAB ? disposition : NEW_FOREGROUND_TAB;
  browser->AddWebContents(
      new_contents, disposition, initial_pos, user_gesture);
}

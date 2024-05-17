void BackgroundContentsService::AddWebContents(
    WebContents* new_contents,
    WindowOpenDisposition disposition,
    const gfx::Rect& initial_pos,
    bool user_gesture) {
  Browser* browser = BrowserList::GetLastActiveWithProfile(
      Profile::FromBrowserContext(new_contents->GetBrowserContext()));
  if (!browser)
    return;
  browser->AddWebContents(new_contents, disposition, initial_pos, user_gesture);
}

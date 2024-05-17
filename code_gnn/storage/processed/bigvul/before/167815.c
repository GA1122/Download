DownloadRequestLimiter::GetDownloadState(
    content::WebContents* web_contents,
    content::WebContents* originating_web_contents,
    bool create) {
  DCHECK(web_contents);
  StateMap::iterator i = state_map_.find(web_contents);
  if (i != state_map_.end())
    return i->second;

  if (!create)
    return nullptr;

  TabDownloadState* state =
      new TabDownloadState(this, web_contents, originating_web_contents);
  state_map_[web_contents] = state;
  return state;
}

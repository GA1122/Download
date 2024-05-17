void DownloadRequestLimiter::Remove(TabDownloadState* state,
                                    content::WebContents* contents) {
  DCHECK(base::ContainsKey(state_map_, contents));
  state_map_.erase(contents);
  delete state;
}

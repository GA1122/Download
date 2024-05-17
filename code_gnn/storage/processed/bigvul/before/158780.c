  bool LoadStateMatches(content::WebContents* contents) {
    DCHECK(contents == web_contents_);
    return waiting_host_ == contents->GetLoadStateHost() &&
           waiting_state_ == contents->GetLoadState().state;
  }

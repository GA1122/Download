  explicit LoadStateWaiter(content::WebContents* contents)
      : web_contents_(contents) {
    contents->SetDelegate(this);
  }

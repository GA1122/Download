  explicit UserInteractionObserver(WebContents* web_contents)
      : WebContentsObserver(web_contents), user_interaction_received_(false) {}

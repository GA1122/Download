  explicit LoadProgressDelegateAndObserver(Shell* shell)
      : WebContentsObserver(shell->web_contents()),
        did_start_loading(false),
        did_stop_loading(false) {
    web_contents()->SetDelegate(this);
  }

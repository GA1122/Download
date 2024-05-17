  DidGetResourceResponseStartObserver(Shell* shell)
      : WebContentsObserver(shell->web_contents()), shell_(shell) {
    shell->web_contents()->SetDelegate(&delegate_);
    EXPECT_FALSE(shell->web_contents()->IsWaitingForResponse());
    EXPECT_FALSE(shell->web_contents()->IsLoading());
  }

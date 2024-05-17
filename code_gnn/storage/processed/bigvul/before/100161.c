  explicit DefaultBrowserInfoBarDelegate(TabContents* contents)
      : ConfirmInfoBarDelegate(contents),
        profile_(contents->profile()),
        action_taken_(false),
        should_expire_(false),
        ALLOW_THIS_IN_INITIALIZER_LIST(method_factory_(this)) {
    MessageLoop::current()->PostDelayedTask(FROM_HERE,
        method_factory_.NewRunnableMethod(
            &DefaultBrowserInfoBarDelegate::Expire),
        8000);   
  }

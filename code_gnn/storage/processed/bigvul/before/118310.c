const content::NavigationController* AutofillDialogViews::ShowSignIn(
    const GURL& url) {
  int min_width = GetContentsBounds().width();
  int min_height = GetDialogClientView()->GetContentsBounds().height();

  sign_in_delegate_.reset(
      new AutofillDialogSignInDelegate(
          this,
          sign_in_web_view_->GetWebContents(),
          delegate_->GetWebContents(),
          gfx::Size(min_width, min_height), GetMaximumSignInViewSize()));
  sign_in_web_view_->LoadInitialURL(url);

  ShowDialogInMode(SIGN_IN);

  ContentsPreferredSizeChanged();

  return &sign_in_web_view_->web_contents()->GetController();
}

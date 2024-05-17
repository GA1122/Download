void ProfileChooserView::ShowViewFromMode(profiles::BubbleViewMode mode) {
  if (SigninViewController::ShouldShowSigninForMode(mode)) {
    Hide();
    browser_->signin_view_controller()->ShowSignin(mode, browser_,
                                                   access_point_);
  } else {
    ShowView(mode, avatar_menu_.get());
  }
}

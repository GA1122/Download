void Browser::ShowModalSigninWindow(profiles::BubbleViewMode mode,
                                    signin_metrics::AccessPoint access_point) {
  signin_view_controller_.ShowModalSignin(mode, this, access_point);
}

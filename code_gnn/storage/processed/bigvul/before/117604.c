bool AutolaunchInfoBarDelegate::Accept() {
  action_taken_ = true;
  auto_launch_trial::UpdateInfobarResponseMetric(
      auto_launch_trial::INFOBAR_OK);
  return true;
}

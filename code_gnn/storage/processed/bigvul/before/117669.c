AutolaunchInfoBarDelegate::~AutolaunchInfoBarDelegate() {
  if (!action_taken_) {
    auto_launch_trial::UpdateInfobarResponseMetric(
        auto_launch_trial::INFOBAR_IGNORE);
  }
}

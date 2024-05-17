bool AutolaunchInfoBarDelegate::Cancel() {
  action_taken_ = true;

  auto_launch_trial::UpdateInfobarResponseMetric(
      auto_launch_trial::INFOBAR_CUT_IT_OUT);
  const bool auto_launch = false;
  auto_launch_trial::UpdateToggleAutoLaunchMetric(auto_launch);

  content::BrowserThread::PostTask(
      content::BrowserThread::FILE, FROM_HERE,
      base::Bind(&auto_launch_util::SetWillLaunchAtLogin,
                 auto_launch, FilePath()));
  return true;
}

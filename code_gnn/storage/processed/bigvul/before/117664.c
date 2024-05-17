bool AutolaunchInfoBarDelegate::ShouldExpire(
    const content::LoadCommittedDetails& details) const {
  return details.is_navigation_to_different_page() && should_expire_;
}

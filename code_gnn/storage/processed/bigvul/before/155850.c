void SupervisedUserService::OnSiteListsChanged(
    const std::vector<scoped_refptr<SupervisedUserSiteList> >& site_lists) {
  whitelists_ = site_lists;
  url_filter_.LoadWhitelists(site_lists);
}

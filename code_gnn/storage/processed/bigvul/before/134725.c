const GURL& GuestViewBase::GetOwnerSiteURL() const {
  return owner_web_contents()->GetLastCommittedURL();
}

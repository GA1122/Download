bool TabHelper::CanCreateBookmarkApp() const {
  return !profile_->IsGuestSession() &&
         !profile_->IsSystemProfile() &&
         IsValidBookmarkAppUrl(web_contents()->GetURL());
}

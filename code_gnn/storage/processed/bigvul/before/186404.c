 void PageInfo::OnChangePasswordButtonPressed(
      content::WebContents* web_contents) {
  #if defined(FULL_SAFE_BROWSING)
    DCHECK(password_protection_service_);
  DCHECK(safe_browsing_status_ == SAFE_BROWSING_STATUS_SIGN_IN_PASSWORD_REUSE ||
         safe_browsing_status_ ==
             SAFE_BROWSING_STATUS_ENTERPRISE_PASSWORD_REUSE);
//   DCHECK(site_identity_status_ == SITE_IDENTITY_STATUS_SIGN_IN_PASSWORD_REUSE ||
//          site_identity_status_ ==
//              SITE_IDENTITY_STATUS_ENTERPRISE_PASSWORD_REUSE);
    password_protection_service_->OnUserAction(
        web_contents,
      safe_browsing_status_ == SAFE_BROWSING_STATUS_SIGN_IN_PASSWORD_REUSE
//       site_identity_status_ == SITE_IDENTITY_STATUS_SIGN_IN_PASSWORD_REUSE
            ? PasswordReuseEvent::SIGN_IN_PASSWORD
            : PasswordReuseEvent::ENTERPRISE_PASSWORD,
        safe_browsing::WarningUIType::PAGE_INFO,
       safe_browsing::WarningAction::CHANGE_PASSWORD);
 #endif
 }
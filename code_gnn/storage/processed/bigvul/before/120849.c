OneClickSigninHelper::OneClickSigninHelper(content::WebContents* web_contents)
    : content::WebContentsObserver(web_contents),
      showing_signin_(false),
      auto_accept_(AUTO_ACCEPT_NONE),
      source_(SyncPromoUI::SOURCE_UNKNOWN),
      switched_to_advanced_(false),
      original_source_(SyncPromoUI::SOURCE_UNKNOWN),
      untrusted_navigations_since_signin_visit_(0),
      untrusted_confirmation_required_(false),
      do_not_clear_pending_email_(false) {
}

void OneClickSigninHelper::ShowInfoBarUIThread(
    const std::string& session_index,
    const std::string& email,
    AutoAccept auto_accept,
    SyncPromoUI::Source source,
    const GURL& continue_url,
    int child_id,
    int route_id) {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));

  content::WebContents* web_contents = tab_util::GetWebContentsByID(child_id,
                                                                    route_id);
  if (!web_contents)
    return;

  OneClickSigninHelper* helper =
      OneClickSigninHelper::FromWebContents(web_contents);
  if (!helper)
    return;

  if (auto_accept != AUTO_ACCEPT_NONE)
    helper->auto_accept_ = auto_accept;

  if (source != SyncPromoUI::SOURCE_UNKNOWN &&
      helper->source_ == SyncPromoUI::SOURCE_UNKNOWN) {
    helper->source_ = source;
  }

  CanOfferFor can_offer_for =
      (auto_accept != AUTO_ACCEPT_EXPLICIT &&
          helper->auto_accept_ != AUTO_ACCEPT_EXPLICIT) ?
          CAN_OFFER_FOR_INTERSTITAL_ONLY : CAN_OFFER_FOR_ALL;

  std::string error_message;

  if (!web_contents || !CanOffer(web_contents, can_offer_for, email,
                                 &error_message)) {
    VLOG(1) << "OneClickSigninHelper::ShowInfoBarUIThread: not offering";
    if (helper && helper->error_message_.empty() && !error_message.empty())
      helper->error_message_ = error_message;

    return;
  }

  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  SigninManager* manager = profile ?
      SigninManagerFactory::GetForProfile(profile) : NULL;
  helper->untrusted_confirmation_required_ |=
      (manager && !manager->IsSigninProcess(child_id));

  if (!session_index.empty())
    helper->session_index_ = session_index;

  if (!email.empty())
    helper->email_ = email;

  if (continue_url.is_valid())
    helper->continue_url_ = continue_url;
}

void OneClickSigninHelper::RedirectToSignin() {
  VLOG(1) << "OneClickSigninHelper::RedirectToSignin";

  SyncPromoUI::Source source =
      SyncPromoUI::GetSourceForSyncPromoURL(continue_url_);
  if (source == SyncPromoUI::SOURCE_UNKNOWN)
    source = SyncPromoUI::SOURCE_MENU;
  GURL page = SyncPromoUI::GetSyncPromoURL(source, false);

  content::WebContents* contents = web_contents();
  contents->GetController().LoadURL(page,
                                    content::Referrer(),
                                    content::PAGE_TRANSITION_AUTO_TOPLEVEL,
                                    std::string());
}

PrefService* ChromePaymentRequestDelegate::GetPrefService() {
  return Profile::FromBrowserContext(web_contents_->GetBrowserContext())
       ->GetPrefs();
 }

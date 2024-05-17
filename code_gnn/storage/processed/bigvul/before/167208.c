void GlobalConfirmInfoBar::MaybeAddInfoBar(content::WebContents* web_contents) {
  InfoBarService* infobar_service =
      InfoBarService::FromWebContents(web_contents);
  DCHECK(infobar_service);
  if (ContainsKey(proxies_, infobar_service))
    return;

  std::unique_ptr<GlobalConfirmInfoBar::DelegateProxy> proxy(
      new GlobalConfirmInfoBar::DelegateProxy(weak_factory_.GetWeakPtr()));
  GlobalConfirmInfoBar::DelegateProxy* proxy_ptr = proxy.get();
  infobars::InfoBar* added_bar = infobar_service->AddInfoBar(
      infobar_service->CreateConfirmInfoBar(std::move(proxy)));

  if (!added_bar) {
    if (!is_closing_) {
      is_closing_ = true;

      base::SequencedTaskRunnerHandle::Get()->PostTask(
          FROM_HERE,
          base::Bind(&GlobalConfirmInfoBar::Close, weak_factory_.GetWeakPtr()));
    }
    return;
  }

  proxy_ptr->info_bar_ = added_bar;
  proxies_[infobar_service] = proxy_ptr;
  infobar_service->AddObserver(this);
}

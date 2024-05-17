void AppListControllerDelegateImpl::OpenURL(Profile* profile,
                                            const GURL& url,
                                            ui::PageTransition transition,
                                            WindowOpenDisposition disposition) {
  chrome::NavigateParams params(profile, url, transition);
  params.disposition = disposition;
  chrome::Navigate(&params);
}

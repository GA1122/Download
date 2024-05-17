void OnLoFiResponseReceivedOnUI(content::WebContents* web_contents) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  PreviewsUITabHelper* ui_tab_helper =
      PreviewsUITabHelper::FromWebContents(web_contents);

  if (!ui_tab_helper)
    return;

  uint64_t page_id = 0;
  if (ui_tab_helper && ui_tab_helper->previews_user_data()) {
    page_id = ui_tab_helper->previews_user_data()->page_id();
  }

  ui_tab_helper->ShowUIElement(
      previews::PreviewsType::LOFI, true  ,
      base::BindOnce(&AddPreviewNavigationToBlackListCallback,
                     web_contents->GetBrowserContext(),
                     web_contents->GetController()
                         .GetLastCommittedEntry()
                         ->GetRedirectChain()[0],
                     previews::PreviewsType::LOFI, page_id));
}

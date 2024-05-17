void Browser::RequestMediaAccessPermissionHelper(
    content::WebContents* web_contents,
    const content::MediaStreamRequest* request,
    const content::MediaResponseCallback& callback) {
  TabContents* tab = TabContents::FromWebContents(web_contents);

  scoped_ptr<MediaStreamDevicesController>
      controller(new MediaStreamDevicesController(tab->profile(),
                                                  request,
                                                  callback));
  if (!controller->DismissInfoBarAndTakeActionOnSettings()) {
  InfoBarTabHelper* infobar_helper =
      InfoBarTabHelper::FromWebContents(web_contents);
    InfoBarDelegate* old_infobar = NULL;
    for (size_t i = 0; i < infobar_helper->GetInfoBarCount(); ++i) {
      old_infobar = infobar_helper->GetInfoBarDelegateAt(i)->
          AsMediaStreamInfoBarDelegate();
      if (old_infobar)
        break;
    }

    InfoBarDelegate* infobar =
        new MediaStreamInfoBarDelegate(infobar_helper, controller.release());
    if (old_infobar)
      infobar_helper->ReplaceInfoBar(old_infobar, infobar);
    else
      infobar_helper->AddInfoBar(infobar);
  }
}

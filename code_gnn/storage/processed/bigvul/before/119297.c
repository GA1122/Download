content::WebContents* TranslateInfoBarDelegate::GetWebContents() {
   return InfoBarService::WebContentsFromInfoBar(infobar());
 }

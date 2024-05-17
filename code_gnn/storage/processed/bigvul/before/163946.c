base::string16 GetPaymentHandlerDialogTitle(
    content::WebContents* web_contents,
    const base::string16& https_prefix) {
  return web_contents == nullptr ||
                 base::StartsWith(web_contents->GetTitle(), https_prefix,
                                  base::CompareCase::SENSITIVE)
             ? base::string16()
             : web_contents->GetTitle();
}

void AwContents::InitAutofillIfNecessary(bool enabled) {
  if (!enabled)
    return;
  content::WebContents* web_contents = web_contents_.get();
  if (ContentAutofillDriverFactory::FromWebContents(web_contents))
    return;

  AwAutofillClient::CreateForWebContents(web_contents);
  ContentAutofillDriverFactory::CreateForWebContentsAndDelegate(
      web_contents, AwAutofillClient::FromWebContents(web_contents),
      base::android::GetDefaultLocale(),
      AutofillManager::DISABLE_AUTOFILL_DOWNLOAD_MANAGER);
}

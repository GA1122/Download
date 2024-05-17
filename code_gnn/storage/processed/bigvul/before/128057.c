void AwContents::SetSaveFormData(bool enabled) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  InitAutofillIfNecessary(enabled);
  if (AwAutofillClient::FromWebContents(web_contents_.get())) {
    AwAutofillClient::FromWebContents(web_contents_.get())->
        SetSaveFormData(enabled);
  }
}

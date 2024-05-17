void BrowsingDataExtensionFunction::OnBrowsingDataRemoverDone() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  this->SendResponse(true);

  Release();   
}

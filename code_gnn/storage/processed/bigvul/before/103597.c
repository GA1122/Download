bool ChromeContentBrowserClient::AllowSaveLocalState(
    const content::ResourceContext& context) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  ProfileIOData* io_data =
      reinterpret_cast<ProfileIOData*>(context.GetUserData(NULL));
  return !io_data->clear_local_state_on_exit()->GetValue();
}

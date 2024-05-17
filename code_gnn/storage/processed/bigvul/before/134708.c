void GuestViewBase::CompleteInit(
    scoped_ptr<base::DictionaryValue> create_params,
    const WebContentsCreatedCallback& callback,
    content::WebContents* guest_web_contents) {
  if (!guest_web_contents) {
    delete this;
    callback.Run(nullptr);
    return;
  }
  InitWithWebContents(*create_params, guest_web_contents);
  callback.Run(guest_web_contents);
}

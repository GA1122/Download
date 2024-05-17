void GuestViewBase::Init(const base::DictionaryValue& create_params,
                         const WebContentsCreatedCallback& callback) {
  if (initialized_)
    return;
  initialized_ = true;

  if (!GuestViewManager::FromBrowserContextIfAvailable(browser_context_)->
          IsGuestAvailableToContext(this, &owner_extension_id_)) {
    delete this;
    callback.Run(nullptr);
    return;
  }

  scoped_ptr<base::DictionaryValue> params(create_params.DeepCopy());
  CreateWebContents(create_params,
                    base::Bind(&GuestViewBase::CompleteInit,
                               weak_ptr_factory_.GetWeakPtr(),
                               base::Passed(&params),
                               callback));
}

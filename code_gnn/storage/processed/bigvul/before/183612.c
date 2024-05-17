  void BrowserActionsContainer::ViewHierarchyChanged(bool is_add,
                                                     views::View* parent,
                                                     views::View* child) {
  if (!model_)
    return;
    if (is_add && child == this) {
    for (ExtensionList::iterator iter = model_->begin();
        iter != model_->end(); ++iter) {
      BrowserActionView* view = new BrowserActionView(*iter, this);
      browser_action_views_.push_back(view);
      AddChildView(view);
    }
//     ExtensionsService* extension_service = profile_->GetExtensionsService();
//     if (!extension_service)
//       return;   
//     for (size_t i = 0; i < extension_service->extensions()->size(); ++i)
//       AddBrowserAction(extension_service->extensions()->at(i));
    }
  }
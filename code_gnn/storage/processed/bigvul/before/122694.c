bool Extension::LoadPageAction(string16* error) {
  DictionaryValue* page_action_value = NULL;

  if (manifest_->HasKey(keys::kPageActions)) {
    ListValue* list_value = NULL;
    if (!manifest_->GetList(keys::kPageActions, &list_value)) {
      *error = ASCIIToUTF16(errors::kInvalidPageActionsList);
      return false;
    }

    size_t list_value_length = list_value->GetSize();

    if (list_value_length == 0u) {
    } else if (list_value_length == 1u) {
      if (!list_value->GetDictionary(0, &page_action_value)) {
        *error = ASCIIToUTF16(errors::kInvalidPageAction);
        return false;
      }
    } else {   
      *error = ASCIIToUTF16(errors::kInvalidPageActionsListSize);
      return false;
    }
  } else if (manifest_->HasKey(keys::kPageAction)) {
    if (!manifest_->GetDictionary(keys::kPageAction, &page_action_value)) {
      *error = ASCIIToUTF16(errors::kInvalidPageAction);
      return false;
    }
  }

  if (page_action_value) {
    page_action_info_ = LoadExtensionActionInfoHelper(
        this, page_action_value, error);
    if (!page_action_info_.get())
      return false;   
  }

  return true;
}

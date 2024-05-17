  syncable::ModelTypeBitSet ParamsMeaningAllEnabledTypes() {
    syncable::ModelTypeBitSet request_params;
    request_params[syncable::BOOKMARKS] = true;
    request_params[syncable::AUTOFILL] = true;
    return request_params;
  }

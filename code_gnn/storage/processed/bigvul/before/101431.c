  syncable::ModelTypeBitSet ParamsMeaningJustOneEnabledType() {
    syncable::ModelTypeBitSet request_params;
    request_params[syncable::AUTOFILL] = true;
    return request_params;
  }

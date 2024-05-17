  void DisableDatatype(syncable::ModelType model_type) {
    enabled_datatypes_[model_type] = false;
    mock_server_->ExpectGetUpdatesRequestTypes(enabled_datatypes_);
  }

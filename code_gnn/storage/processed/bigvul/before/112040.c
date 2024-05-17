  void EnableDatatype(syncable::ModelType model_type) {
    enabled_datatypes_.Put(model_type);
    mock_server_->ExpectGetUpdatesRequestTypes(enabled_datatypes_);
  }

  void DisableDatatype(syncable::ModelType model_type) {
    enabled_datatypes_.Remove(model_type);
    mock_server_->ExpectGetUpdatesRequestTypes(enabled_datatypes_);
  }

  void EnableDatatype(syncable::ModelType model_type) {
    enabled_datatypes_[model_type] = true;
    mock_server_->ExpectGetUpdatesRequestTypes(enabled_datatypes_);
  }

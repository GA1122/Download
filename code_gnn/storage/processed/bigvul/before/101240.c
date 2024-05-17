  void Add(const Id& id) {
    int revision = next_revision_++;
    mock_server_->AddUpdateDirectory(id.GetServerId(), kRootId,
        id.GetServerId(), revision, revision);
    mock_server_->SetLastUpdatePosition(90210);
  }

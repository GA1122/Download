  void Update(BytesConsumer* consumer) {
    DCHECK(!underlying_);
    if (is_cancelled_) {
      return;
    }

    underlying_ = consumer;
    if (client_) {
      Client* client = client_;
      client_ = nullptr;
      underlying_->SetClient(client);
      if (GetPublicState() != PublicState::kReadableOrWaiting)
        client->OnStateChange();
    }
  }

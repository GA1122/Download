  base::WeakPtr<FakeStream> FinishStreamRequest() {
    FakeStream* fake_stream = new FakeStream(priority_);
    base::WeakPtr<FakeStream> weak_stream = fake_stream->AsWeakPtr();
    delegate_->OnStreamReady(SSLConfig(), ProxyInfo(), fake_stream);
    return weak_stream;
  }

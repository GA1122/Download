  AudioOutputStreamPtr CreateAudioOutput() {
    AudioOutputStreamPtr p;
    ExpectDelegateCreation();
    impl_ = std::make_unique<MojoAudioOutputStream>(
        mojo::MakeRequest(&p), std::move(client_ptr_),
        base::BindOnce(&MockDelegateFactory::CreateDelegate,
                       base::Unretained(&mock_delegate_factory_)),
        base::BindOnce(&MockClient::Initialized, base::Unretained(&client_)),
        base::BindOnce(&MockDeleter::Finished, base::Unretained(&deleter_)));
    EXPECT_TRUE(p.is_bound());
    return p;
  }

  void ExpectDelegateCreation() {
    delegate_ = new StrictMock<MockDelegate>();
    mock_delegate_factory_.PrepareDelegateForCreation(
        base::WrapUnique(delegate_));
    EXPECT_TRUE(
        base::CancelableSyncSocket::CreatePair(&local_, foreign_socket_.get()));
    base::SharedMemoryCreateOptions shmem_options;
    shmem_options.size = kShmemSize;
    shmem_options.share_read_only = true;
    EXPECT_TRUE(mem_.Create(shmem_options));
    EXPECT_CALL(mock_delegate_factory_, MockCreateDelegate(NotNull()))
        .WillOnce(SaveArg<0>(&delegate_event_handler_));
  }

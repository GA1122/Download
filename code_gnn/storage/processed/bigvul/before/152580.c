  typename T::Param ProcessAndReadIPC() {
    base::RunLoop().RunUntilIdle();
    const IPC::Message* message =
        render_thread_->sink().GetUniqueMessageMatching(T::ID);
    typename T::Param param;
    EXPECT_TRUE(message);
    if (message)
      T::Read(message, &param);
    return param;
  }

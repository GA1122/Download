  void ReceiveDataLow(int size) {
    EXPECT_TRUE(active_loader());
    std::unique_ptr<char[]> data(new char[size]);
    memset(data.get(), 0xA5, size);   

    data_provider()->DidReceiveData(data.get(), size);
  }

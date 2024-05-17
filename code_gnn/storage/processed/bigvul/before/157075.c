  void ReceiveData(int size) {
    ReceiveDataLow(size);
    base::RunLoop().RunUntilIdle();
  }

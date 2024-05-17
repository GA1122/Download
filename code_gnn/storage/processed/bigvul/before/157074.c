  void ReadAt(int64_t position, int64_t howmuch = kDataSize) {
    data_source_->Read(position, howmuch, buffer_,
                       base::Bind(&MultibufferDataSourceTest::ReadCallback,
                                  base::Unretained(this)));
    base::RunLoop().RunUntilIdle();
  }

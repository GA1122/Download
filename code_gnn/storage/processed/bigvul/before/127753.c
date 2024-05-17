void TestTransactionConsumer::Read() {
  state_ = READING;
  read_buf_ = new IOBuffer(1024);
  int result = trans_->Read(read_buf_.get(),
                            1024,
                            base::Bind(&TestTransactionConsumer::OnIOComplete,
                                       base::Unretained(this)));
  if (result != ERR_IO_PENDING)
    DidRead(result);
}

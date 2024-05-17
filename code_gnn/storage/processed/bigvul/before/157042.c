void MultibufferDataSource::ReadOperation::Run(
    std::unique_ptr<ReadOperation> read_op,
    int result) {
  std::move(read_op->callback_).Run(result);
}

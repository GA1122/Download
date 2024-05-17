MultibufferDataSource::ReadOperation::ReadOperation(
    int64_t position,
    int size,
    uint8_t* data,
    const DataSource::ReadCB& callback)
    : position_(position), size_(size), data_(data), callback_(callback) {
  DCHECK(!callback_.is_null());
}

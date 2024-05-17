int NaClIPCAdapter::LockedReceive(char* output_buffer,
                                  size_t output_buffer_size) {
  lock_.AssertAcquired();

  if (locked_data_.to_be_received_.empty())
    return 0;
  scoped_refptr<RewrittenMessage> current =
      locked_data_.to_be_received_.front();

  int retval = current->Read(output_buffer, output_buffer_size);

  if (current->is_consumed())
    locked_data_.to_be_received_.pop();
  return retval;
}

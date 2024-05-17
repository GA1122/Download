int NaClIPCAdapter::BlockingReceive(char* output_buffer,
                                    size_t output_buffer_size) {
  int retval = 0;
  {
    base::AutoLock lock(lock_);
    while (locked_data_.to_be_received_.empty() &&
           !locked_data_.channel_closed_)
      cond_var_.Wait();
    if (locked_data_.channel_closed_) {
      retval = -1;
    } else {
      retval = LockedReceive(output_buffer, output_buffer_size);
      DCHECK(retval > 0);
    }
  }
  cond_var_.Signal();
  return retval;
}

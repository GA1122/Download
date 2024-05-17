void VaapiVideoDecodeAccelerator::MapAndQueueNewInputBuffer(
    const media::BitstreamBuffer& bitstream_buffer) {
  DCHECK_EQ(message_loop_, base::MessageLoop::current());
  TRACE_EVENT1("Video Decoder", "MapAndQueueNewInputBuffer", "input_id",
      bitstream_buffer.id());

  DVLOG(4) << "Mapping new input buffer id: " << bitstream_buffer.id()
           << " size: " << (int)bitstream_buffer.size();

  scoped_ptr<base::SharedMemory> shm(
      new base::SharedMemory(bitstream_buffer.handle(), true));
  RETURN_AND_NOTIFY_ON_FAILURE(shm->Map(bitstream_buffer.size()),
                              "Failed to map input buffer", UNREADABLE_INPUT,);

  base::AutoLock auto_lock(lock_);

  linked_ptr<InputBuffer> input_buffer(new InputBuffer());
  input_buffer->shm.reset(shm.release());
  input_buffer->id = bitstream_buffer.id();
  input_buffer->size = bitstream_buffer.size();

  ++num_stream_bufs_at_decoder_;
  TRACE_COUNTER1("Video Decoder", "Stream buffers at decoder",
                 num_stream_bufs_at_decoder_);

  input_buffers_.push(input_buffer);
  input_ready_.Signal();
}

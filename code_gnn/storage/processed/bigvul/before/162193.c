void VideoCaptureImpl::OnClientBufferFinished(
    int buffer_id,
    scoped_refptr<ClientBuffer> buffer,
    double consumer_resource_utilization) {
  DCHECK(io_thread_checker_.CalledOnValidThread());

#if DCHECK_IS_ON()
  DCHECK(!buffer->HasOneRef());
  ClientBuffer* const buffer_raw_ptr = buffer.get();
  buffer = nullptr;
  DCHECK(buffer_raw_ptr->HasOneRef());
#else
  buffer = nullptr;
#endif

  GetVideoCaptureHost()->ReleaseBuffer(
      device_id_, buffer_id, consumer_resource_utilization);
}

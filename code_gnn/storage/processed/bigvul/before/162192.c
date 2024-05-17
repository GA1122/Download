void VideoCaptureImpl::OnBufferReady(int32_t buffer_id,
                                     media::mojom::VideoFrameInfoPtr info) {
  DVLOG(1) << __func__ << " buffer_id: " << buffer_id;
  DCHECK(io_thread_checker_.CalledOnValidThread());

  bool consume_buffer = state_ == VIDEO_CAPTURE_STATE_STARTED;
  if ((info->pixel_format != media::PIXEL_FORMAT_I420 &&
       info->pixel_format != media::PIXEL_FORMAT_Y16) ||
      info->storage_type != media::VideoPixelStorage::CPU) {
    consume_buffer = false;
    LOG(DFATAL) << "Wrong pixel format or storage, got pixel format:"
                << VideoPixelFormatToString(info->pixel_format)
                << ", storage:" << static_cast<int>(info->storage_type);
  }
  if (!consume_buffer) {
    GetVideoCaptureHost()->ReleaseBuffer(device_id_, buffer_id, -1.0);
    return;
  }

  base::TimeTicks reference_time;
  media::VideoFrameMetadata frame_metadata;
  frame_metadata.MergeInternalValuesFrom(*info->metadata);
  const bool success = frame_metadata.GetTimeTicks(
      media::VideoFrameMetadata::REFERENCE_TIME, &reference_time);
  DCHECK(success);

  if (first_frame_ref_time_.is_null())
    first_frame_ref_time_ = reference_time;

  if (info->timestamp.is_zero())
    info->timestamp = reference_time - first_frame_ref_time_;

  TRACE_EVENT_INSTANT2("cast_perf_test", "OnBufferReceived",
                       TRACE_EVENT_SCOPE_THREAD, "timestamp",
                       (reference_time - base::TimeTicks()).InMicroseconds(),
                       "time_delta", info->timestamp.InMicroseconds());

  const auto& iter = client_buffers_.find(buffer_id);
  DCHECK(iter != client_buffers_.end());
  scoped_refptr<ClientBuffer> buffer = iter->second;
  scoped_refptr<media::VideoFrame> frame =
      media::VideoFrame::WrapExternalSharedMemory(
          static_cast<media::VideoPixelFormat>(info->pixel_format),
          info->coded_size, info->visible_rect, info->visible_rect.size(),
          reinterpret_cast<uint8_t*>(buffer->buffer()->memory()),
          buffer->buffer_size(), buffer->buffer()->handle(),
          0  , info->timestamp);
  if (!frame) {
    GetVideoCaptureHost()->ReleaseBuffer(device_id_, buffer_id, -1.0);
    return;
  }

  frame->AddDestructionObserver(base::BindOnce(
      &VideoCaptureImpl::DidFinishConsumingFrame, frame->metadata(),
      media::BindToCurrentLoop(base::BindOnce(
          &VideoCaptureImpl::OnClientBufferFinished, weak_factory_.GetWeakPtr(),
          buffer_id, std::move(buffer)))));

  frame->metadata()->MergeInternalValuesFrom(*info->metadata);

  for (const auto& client : clients_)
    client.second.deliver_frame_cb.Run(frame, reference_time);
}

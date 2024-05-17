   void SimulateBufferReceived(int buffer_id, const gfx::Size& size) {
    media::mojom::VideoFrameInfoPtr info = media::mojom::VideoFrameInfo::New();

    const base::TimeTicks now = base::TimeTicks::Now();
    media::VideoFrameMetadata frame_metadata;
    frame_metadata.SetTimeTicks(media::VideoFrameMetadata::REFERENCE_TIME, now);
    info->metadata = frame_metadata.CopyInternalValues();

    info->timestamp = now - base::TimeTicks();
    info->pixel_format = media::PIXEL_FORMAT_I420;
    info->storage_type = media::VideoPixelStorage::CPU;
    info->coded_size = size;
    info->visible_rect = gfx::Rect(size);

    video_capture_impl_->OnBufferReady(buffer_id, std::move(info));
  }

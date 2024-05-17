size_t V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::FinalizeJpegImage(
    scoped_refptr<VideoFrame> output_frame,
    size_t buffer_size,
    std::unique_ptr<UnalignedSharedMemory> exif_shm) {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  size_t idx = 0;

  auto output_gmb_handle = CreateGpuMemoryBufferHandle(output_frame.get());
  if (output_gmb_handle.is_null()) {
    VLOGF(1) << "Failed to create GpuMemoryBufferHandle";
    return 0;
  }
  auto output_gmb_buffer =
      gpu_memory_buffer_support_->CreateGpuMemoryBufferImplFromHandle(
          std::move(output_gmb_handle), output_frame->coded_size(),
          gfx::BufferFormat::R_8, gfx::BufferUsage::SCANOUT_CAMERA_READ_WRITE,
          base::DoNothing());

  bool isMapped = output_gmb_buffer->Map();
  if (!isMapped) {
    VLOGF(1) << "Failed to map gmb buffer";
    return 0;
  }
  uint8_t* dst_ptr = static_cast<uint8_t*>(output_gmb_buffer->memory(0));

  static const uint8_t kJpegStart[] = {0xFF, JPEG_SOI};

  if (exif_shm) {
    uint8_t* exif_buffer = static_cast<uint8_t*>(exif_shm->memory());
    size_t exif_buffer_size = exif_shm->size();
    uint16_t exif_segment_size = static_cast<uint16_t>(exif_buffer_size + 2);
    const uint8_t kAppSegment[] = {
        0xFF, JPEG_APP1, static_cast<uint8_t>(exif_segment_size / 256),
        static_cast<uint8_t>(exif_segment_size % 256)};

    size_t compressed_data_offset = sizeof(kJpegStart) + sizeof(kAppSegment) +
                                    exif_buffer_size + jpeg_markers_.size();
    memmove(dst_ptr + compressed_data_offset, dst_ptr, buffer_size);

    memcpy(dst_ptr, kJpegStart, sizeof(kJpegStart));
    idx += sizeof(kJpegStart);
    memcpy(dst_ptr + idx, kAppSegment, sizeof(kAppSegment));
    idx += sizeof(kAppSegment);
    memcpy(dst_ptr + idx, exif_buffer, exif_buffer_size);
    idx += exif_buffer_size;
  } else {
    static const uint8_t kAppSegment[] = {
        0xFF, JPEG_APP0, 0x00,
        0x10,   
        0x4A,   
        0x46,   
        0x49,   
        0x46,   
        0x00,   
        0x01,   
        0x01,   
        0x01,   
        0x00,
        0x48,   
        0x00,
        0x48,   
        0x00,   
        0x00    
    };

    size_t compressed_data_offset =
        sizeof(kJpegStart) + sizeof(kAppSegment) + jpeg_markers_.size();
    memmove(dst_ptr + compressed_data_offset, dst_ptr, buffer_size);

    memcpy(dst_ptr, kJpegStart, sizeof(kJpegStart));
    idx += sizeof(kJpegStart);

    memcpy(dst_ptr + idx, kAppSegment, sizeof(kAppSegment));
    idx += sizeof(kAppSegment);
  }

  switch (output_buffer_pixelformat_) {
    case V4L2_PIX_FMT_JPEG_RAW:
      memcpy(dst_ptr + idx, jpeg_markers_.data(), jpeg_markers_.size());
      idx += jpeg_markers_.size();
      idx += buffer_size;
      if (dst_ptr[idx - 2] != 0xFF && dst_ptr[idx - 1] != JPEG_EOI) {
        dst_ptr[idx] = 0xFF;
        dst_ptr[idx + 1] = JPEG_EOI;
        idx += 2;
      }
      break;

    default:
      NOTREACHED() << "Unsupported output pixel format";
  }

  output_gmb_buffer->Unmap();

  return idx;
}

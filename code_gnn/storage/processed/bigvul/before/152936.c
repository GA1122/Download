    Read(media::mojom::VideoCaptureFormatDataView data,
         media::VideoCaptureFormat* out) {
  if (!data.ReadFrameSize(&out->frame_size))
    return false;
  out->frame_rate = data.frame_rate();
  if (!data.ReadPixelFormat(&out->pixel_format))
    return false;
  return true;
}

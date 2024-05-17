unsigned int VaSurfaceFormatForJpeg(const JpegFrameHeader& frame_header) {
  if (frame_header.num_components != 3)
    return kInvalidVaRtFormat;

  const uint8_t y_h = frame_header.components[0].horizontal_sampling_factor;
  const uint8_t y_v = frame_header.components[0].vertical_sampling_factor;
  const uint8_t u_h = frame_header.components[1].horizontal_sampling_factor;
  const uint8_t u_v = frame_header.components[1].vertical_sampling_factor;
  const uint8_t v_h = frame_header.components[2].horizontal_sampling_factor;
  const uint8_t v_v = frame_header.components[2].vertical_sampling_factor;

  if (u_h != 1 || u_v != 1 || v_h != 1 || v_v != 1)
    return kInvalidVaRtFormat;

  if (y_h == 2 && y_v == 2)
    return VA_RT_FORMAT_YUV420;
  else if (y_h == 2 && y_v == 1)
    return VA_RT_FORMAT_YUV422;
  else if (y_h == 1 && y_v == 1)
    return VA_RT_FORMAT_YUV444;
  return kInvalidVaRtFormat;
}

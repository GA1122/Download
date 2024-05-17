vmnc_fill_buffer (GstVMncDec * dec, GstVideoCodecFrame * frame)
{
  GstFlowReturn ret;
  GstMapInfo map;

  ret =
      gst_video_decoder_allocate_output_frame (GST_VIDEO_DECODER (dec), frame);
  if (ret != GST_FLOW_OK)
    return ret;

  gst_buffer_map (frame->output_buffer, &map, GST_MAP_READWRITE);

  memcpy (map.data, dec->imagedata, map.size);

  if (dec->cursor.visible)
    render_cursor (dec, map.data);

  gst_buffer_unmap (frame->output_buffer, &map);

  return GST_FLOW_OK;
}

gst_vmnc_dec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame)
{
  GstVMncDec *dec = GST_VMNC_DEC (decoder);
  int res;
  GstFlowReturn ret = GST_FLOW_OK;
  GstMapInfo map;

  if (!gst_buffer_map (frame->input_buffer, &map, GST_MAP_READ))
    return GST_FLOW_ERROR;

  res = vmnc_handle_packet (dec, map.data, map.size, TRUE);

  gst_buffer_unmap (frame->input_buffer, &map);

  if (!dec->have_format) {
    GST_VIDEO_DECODER_ERROR (decoder, 2, STREAM, DECODE, (NULL),
        ("Data found before header"), ret);
    gst_video_decoder_drop_frame (decoder, frame);
  } else if (res < 0) {
    ret = GST_FLOW_ERROR;
    gst_video_decoder_drop_frame (decoder, frame);
    GST_VIDEO_DECODER_ERROR (decoder, 1, STREAM, DECODE, (NULL),
        ("Couldn't decode packet"), ret);
  } else {
    GST_LOG_OBJECT (dec, "read %d bytes of %" G_GSIZE_FORMAT, res,
        gst_buffer_get_size (frame->input_buffer));
     
    ret = vmnc_fill_buffer (dec, frame);
    if (ret == GST_FLOW_OK)
      gst_video_decoder_finish_frame (decoder, frame);
    else
      gst_video_decoder_drop_frame (decoder, frame);
  }

  return ret;
}

gst_vmnc_dec_parse (GstVideoDecoder * decoder, GstVideoCodecFrame * frame,
    GstAdapter * adapter, gboolean at_eos)
{
  GstVMncDec *dec = GST_VMNC_DEC (decoder);
  const guint8 *data;
  int avail;
  int len;

  avail = gst_adapter_available (adapter);
  data = gst_adapter_map (adapter, avail);

  GST_LOG_OBJECT (dec, "Parsing %d bytes", avail);

  len = vmnc_handle_packet (dec, data, avail, FALSE);

  if (len == ERROR_INSUFFICIENT_DATA) {
    GST_LOG_OBJECT (dec, "Not enough data yet");
    return GST_VIDEO_DECODER_FLOW_NEED_DATA;
  } else if (len < 0) {
    GST_ERROR_OBJECT (dec, "Fatal error in bitstream");
    return GST_FLOW_ERROR;
  } else {
    GST_LOG_OBJECT (dec, "Parsed packet: %d bytes", len);
    gst_video_decoder_add_to_frame (decoder, len);
    return gst_video_decoder_have_frame (decoder);
  }
}

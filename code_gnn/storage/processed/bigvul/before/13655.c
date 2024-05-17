gst_vmnc_dec_class_init (GstVMncDecClass * klass)
{
  GstElementClass *gstelement_class = GST_ELEMENT_CLASS (klass);
  GstVideoDecoderClass *decoder_class = GST_VIDEO_DECODER_CLASS (klass);

  decoder_class->start = gst_vmnc_dec_reset;
  decoder_class->stop = gst_vmnc_dec_reset;
  decoder_class->parse = gst_vmnc_dec_parse;
  decoder_class->handle_frame = gst_vmnc_dec_handle_frame;
  decoder_class->set_format = gst_vmnc_dec_set_format;
  decoder_class->sink_event = gst_vmnc_dec_sink_event;

  gst_element_class_add_static_pad_template (gstelement_class,
      &vmnc_dec_src_factory);
  gst_element_class_add_static_pad_template (gstelement_class,
      &vmnc_dec_sink_factory);
  gst_element_class_set_static_metadata (gstelement_class, "VMnc video decoder",
      "Codec/Decoder/Video", "Decode VmWare video to raw (RGB) video",
      "Michael Smith <msmith@xiph.org>");

  GST_DEBUG_CATEGORY_INIT (vmnc_debug, "vmncdec", 0, "VMnc decoder");
}

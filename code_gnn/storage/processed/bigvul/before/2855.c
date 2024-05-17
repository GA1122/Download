gst_pngdec_init (GstPngDec * pngdec)
{
  pngdec->sinkpad =
      gst_pad_new_from_static_template (&gst_pngdec_sink_pad_template, "sink");
  gst_pad_set_activate_function (pngdec->sinkpad, gst_pngdec_sink_activate);
  gst_pad_set_activatepush_function (pngdec->sinkpad,
      gst_pngdec_sink_activate_push);
  gst_pad_set_activatepull_function (pngdec->sinkpad,
      gst_pngdec_sink_activate_pull);
  gst_pad_set_chain_function (pngdec->sinkpad, gst_pngdec_chain);
  gst_pad_set_event_function (pngdec->sinkpad, gst_pngdec_sink_event);
  gst_pad_set_setcaps_function (pngdec->sinkpad, gst_pngdec_sink_setcaps);
  gst_element_add_pad (GST_ELEMENT (pngdec), pngdec->sinkpad);

  pngdec->srcpad =
      gst_pad_new_from_static_template (&gst_pngdec_src_pad_template, "src");
  gst_pad_use_fixed_caps (pngdec->srcpad);
  gst_element_add_pad (GST_ELEMENT (pngdec), pngdec->srcpad);

  pngdec->buffer_out = NULL;
  pngdec->png = NULL;
  pngdec->info = NULL;
  pngdec->endinfo = NULL;
  pngdec->setup = FALSE;

  pngdec->color_type = -1;
  pngdec->width = -1;
  pngdec->height = -1;
  pngdec->bpp = -1;
  pngdec->fps_n = 0;
  pngdec->fps_d = 1;

  pngdec->in_timestamp = GST_CLOCK_TIME_NONE;
  pngdec->in_duration = GST_CLOCK_TIME_NONE;

  gst_segment_init (&pngdec->segment, GST_FORMAT_UNDEFINED);

  pngdec->image_ready = FALSE;
}

gst_asf_demux_check_activate_streams (GstASFDemux * demux, gboolean force)
{
  guint i, actual_streams = 0;

  if (demux->activated_streams)
    return TRUE;

  if (G_UNLIKELY (!gst_asf_demux_check_first_ts (demux, force)))
    return FALSE;

  if (!all_streams_prerolled (demux) && !force) {
    GST_DEBUG_OBJECT (demux, "not all streams with data beyond preroll yet");
    return FALSE;
  }

  for (i = 0; i < demux->num_streams; ++i) {
    AsfStream *stream = &demux->stream[i];

    if (stream->payloads->len > 0) {

      if (stream->inspect_payload &&     
          !stream->active &&     
          !gst_asf_demux_update_caps_from_payload (demux, stream) &&     
          stream->payloads->len < 20) {  
         
        return FALSE;
      }
       
      GST_LOG_OBJECT (stream->pad, "is prerolled - activate!");
      gst_asf_demux_activate_stream (demux, stream);
      actual_streams += 1;
    } else {
      GST_LOG_OBJECT (stream->pad, "no data, ignoring stream");
    }
  }

  if (actual_streams == 0) {
     
    GST_ERROR_OBJECT (demux, "No streams activated!");
    return FALSE;
  }

  gst_asf_demux_release_old_pads (demux);

  demux->activated_streams = TRUE;
  GST_LOG_OBJECT (demux, "signalling no more pads");
  gst_element_no_more_pads (GST_ELEMENT (demux));
  return TRUE;
}

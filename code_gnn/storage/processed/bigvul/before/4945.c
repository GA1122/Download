gst_qtdemux_handle_sink_event (GstPad * sinkpad, GstEvent * event)
{
  GstQTDemux *demux = GST_QTDEMUX (GST_PAD_PARENT (sinkpad));
  gboolean res;

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_NEWSEGMENT:
       
      gst_event_unref (event);
      res = TRUE;
      break;
    case GST_EVENT_EOS:
       
      if (!demux->pullbased && demux->n_streams == 0) {
        GST_ELEMENT_ERROR (demux, STREAM, DECODE,
            (_("This file contains no playable streams.")),
            ("no known streams found"));
      }
       
    default:
      res = gst_pad_event_default (demux->sinkpad, event);
      break;
  }

  return res;
}

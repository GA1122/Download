gst_asf_demux_check_first_ts (GstASFDemux * demux, gboolean force)
{
  if (G_UNLIKELY (!GST_CLOCK_TIME_IS_VALID (demux->first_ts))) {
    GstClockTime first_ts = GST_CLOCK_TIME_NONE;
    int i;

     
    for (i = 0; i < demux->num_streams; ++i) {
      AsfStream *stream;
      int j;
      GstClockTime stream_min_ts = GST_CLOCK_TIME_NONE;
      GstClockTime stream_min_ts2 = GST_CLOCK_TIME_NONE;         
      stream = &demux->stream[i];

      for (j = 0; j < stream->payloads->len; ++j) {
        AsfPayload *payload = &g_array_index (stream->payloads, AsfPayload, j);
        if (GST_CLOCK_TIME_IS_VALID (payload->ts) &&
            (!GST_CLOCK_TIME_IS_VALID (stream_min_ts)
                || stream_min_ts > payload->ts)) {
          stream_min_ts = payload->ts;
        }
        if (GST_CLOCK_TIME_IS_VALID (payload->ts) &&
            payload->ts > stream_min_ts &&
            (!GST_CLOCK_TIME_IS_VALID (stream_min_ts2)
                || stream_min_ts2 > payload->ts)) {
          stream_min_ts2 = payload->ts;
        }
      }

       

      if (stream_min_ts == 0 && stream_min_ts2 == GST_CLOCK_TIME_NONE && !force)         
        return FALSE;

      if (stream_min_ts == 0 && stream_min_ts2 > GST_SECOND)     
        stream_min_ts = stream_min_ts2;

       
      if (!GST_CLOCK_TIME_IS_VALID (stream_min_ts) && !force)
        return FALSE;

      if (GST_CLOCK_TIME_IS_VALID (stream_min_ts) &&
          (!GST_CLOCK_TIME_IS_VALID (first_ts) || first_ts > stream_min_ts))
        first_ts = stream_min_ts;
    }

    if (!GST_CLOCK_TIME_IS_VALID (first_ts))     
      first_ts = 0;

    demux->first_ts = first_ts;

     
    for (i = 0; i < demux->num_streams; ++i) {
      AsfStream *stream;
      int j;
      stream = &demux->stream[i];

      for (j = 0; j < stream->payloads->len; ++j) {
        AsfPayload *payload = &g_array_index (stream->payloads, AsfPayload, j);
        if (GST_CLOCK_TIME_IS_VALID (payload->ts)) {
          if (payload->ts > first_ts)
            payload->ts -= first_ts;
          else
            payload->ts = 0;
        }
      }
    }
  }

  gst_asf_demux_check_segment_ts (demux, 0);

  return TRUE;
}

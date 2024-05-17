gst_asf_demux_find_stream_with_complete_payload (GstASFDemux * demux)
{
  AsfPayload *best_payload = NULL;
  AsfStream *best_stream = NULL;
  guint i;

  for (i = 0; i < demux->num_streams; ++i) {
    AsfStream *stream;
    int j;

    stream = &demux->stream[i];

     
    if (stream->payloads->len > 0) {
      AsfPayload *payload = NULL;
      gint last_idx;

      if (GST_ASF_DEMUX_IS_REVERSE_PLAYBACK (demux->segment)) {
         

        if (stream->is_video) {
           
          if (stream->reverse_kf_ready) {
            payload =
                &g_array_index (stream->payloads, AsfPayload, stream->kf_pos);
            if (G_UNLIKELY (!GST_CLOCK_TIME_IS_VALID (payload->ts))) {
               
              continue;
            }
          } else {
            continue;
          }
        } else {
           
          for (j = stream->payloads->len - 1;
              j >= 0 && (payload == NULL
                  || !GST_CLOCK_TIME_IS_VALID (payload->ts)); --j) {
            payload = &g_array_index (stream->payloads, AsfPayload, j);
          }

           
          if (!gst_asf_payload_is_complete (payload))
            continue;

        }
      } else {

         
        for (last_idx = stream->payloads->len - 1;
            last_idx >= 0 && (payload == NULL
                || !GST_CLOCK_TIME_IS_VALID (payload->ts)); --last_idx) {
          payload = &g_array_index (stream->payloads, AsfPayload, last_idx);
        }

         
        if (GST_CLOCK_TIME_IS_VALID (payload->ts))
          gst_asf_demux_check_segment_ts (demux, payload->ts);

        if (G_UNLIKELY (GST_CLOCK_TIME_IS_VALID (payload->ts) &&
                (payload->ts < demux->segment.start))) {
          if (G_UNLIKELY ((!demux->keyunit_sync) && (!demux->accurate)
                  && payload->keyframe)) {
            GST_DEBUG_OBJECT (stream->pad,
                "Found keyframe, updating segment start to %" GST_TIME_FORMAT,
                GST_TIME_ARGS (payload->ts));
            demux->segment.start = payload->ts;
            demux->segment.time = payload->ts;
          } else {
            GST_DEBUG_OBJECT (stream->pad, "Last queued payload has timestamp %"
                GST_TIME_FORMAT " which is before our segment start %"
                GST_TIME_FORMAT ", not pushing yet",
                GST_TIME_ARGS (payload->ts),
                GST_TIME_ARGS (demux->segment.start));
            continue;
          }
        }
        payload = NULL;
         
        for (j = 0;
            j < stream->payloads->len && (payload == NULL
                || !GST_CLOCK_TIME_IS_VALID (payload->ts)); ++j) {
          payload = &g_array_index (stream->payloads, AsfPayload, j);
        }

         
        if (!gst_asf_payload_is_complete (payload))
          continue;
      }

       
      if (best_stream == NULL || best_payload->ts > payload->ts) {
        best_stream = stream;
        best_payload = payload;
      }
    }
  }

  return best_stream;
}

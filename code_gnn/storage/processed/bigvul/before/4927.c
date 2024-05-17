gst_qtdemux_activate_segment (GstQTDemux * qtdemux, QtDemuxStream * stream,
    guint32 seg_idx, guint64 offset)
{
  GstEvent *event;
  QtDemuxSegment *segment;
  guint32 index, kf_index;
  guint64 seg_time;
  guint64 start, stop, time;
  gdouble rate;

  GST_LOG_OBJECT (qtdemux, "activate segment %d, offset %" G_GUINT64_FORMAT,
      seg_idx, offset);

   
  stream->segment_index = seg_idx;

   
  segment = &stream->segments[seg_idx];

  if (offset < segment->time) {
    GST_WARNING_OBJECT (qtdemux, "offset < segment->time %" G_GUINT64_FORMAT,
        segment->time);
    return FALSE;
  }

   
  seg_time = offset - segment->time;

  GST_LOG_OBJECT (qtdemux, "seg_time %" GST_TIME_FORMAT,
      GST_TIME_ARGS (seg_time));

  if (seg_time > segment->duration) {
    GST_LOG_OBJECT (qtdemux, "seg_time > segment->duration %" GST_TIME_FORMAT,
        GST_TIME_ARGS (segment->duration));
    return FALSE;
  }

   

  if (qtdemux->segment.stop == -1)
    stop = segment->media_stop;
  else
    stop =
        MIN (segment->media_stop,
        qtdemux->segment.stop - segment->time + segment->media_start);

  if (qtdemux->segment.rate >= 0) {
    start = MIN (segment->media_start + seg_time, stop);
    time = offset;
  } else {
    start = segment->media_start;
    stop = MIN (segment->media_start + seg_time, stop);
    time = segment->time;
  }

  GST_DEBUG_OBJECT (qtdemux, "newsegment %d from %" GST_TIME_FORMAT
      " to %" GST_TIME_FORMAT ", time %" GST_TIME_FORMAT, seg_idx,
      GST_TIME_ARGS (start), GST_TIME_ARGS (stop), GST_TIME_ARGS (time));

   
  rate = segment->rate * qtdemux->segment.rate;

   
  gst_segment_init (&stream->segment, GST_FORMAT_TIME);
  gst_segment_set_newsegment (&stream->segment, FALSE, rate, GST_FORMAT_TIME,
      start, stop, time);

   
  if (stream->pad) {
    event = gst_event_new_new_segment (FALSE, rate, GST_FORMAT_TIME,
        start, stop, time);
    gst_pad_push_event (stream->pad, event);
     
    stream->last_ret = GST_FLOW_OK;
  }

   
  if (qtdemux->segment.rate >= 0) {
    index = gst_qtdemux_find_index (qtdemux, stream, start);
    stream->to_sample = stream->n_samples;
    GST_DEBUG_OBJECT (qtdemux, "moving data pointer to %" GST_TIME_FORMAT
        ", index: %u, pts %" GST_TIME_FORMAT, GST_TIME_ARGS (start), index,
        GST_TIME_ARGS (stream->samples[index].timestamp));
  } else {
    index = gst_qtdemux_find_index (qtdemux, stream, stop);
    stream->to_sample = index;
    GST_DEBUG_OBJECT (qtdemux, "moving data pointer to %" GST_TIME_FORMAT
        ", index: %u, pts %" GST_TIME_FORMAT, GST_TIME_ARGS (stop), index,
        GST_TIME_ARGS (stream->samples[index].timestamp));
  }

   
  if (index == stream->sample_index) {
    GST_DEBUG_OBJECT (qtdemux, "we are at the right index");
    return TRUE;
  }

   
  kf_index = gst_qtdemux_find_keyframe (qtdemux, stream, index);

   
  if (index > stream->sample_index) {
     
    if (kf_index > stream->sample_index) {
      GST_DEBUG_OBJECT (qtdemux, "moving forwards to keyframe at %u (pts %"
          GST_TIME_FORMAT, kf_index,
          GST_TIME_ARGS (stream->samples[kf_index].timestamp));
      gst_qtdemux_move_stream (qtdemux, stream, kf_index);
    } else {
      GST_DEBUG_OBJECT (qtdemux, "moving forwards, keyframe at %u (pts %"
          GST_TIME_FORMAT " already sent", kf_index,
          GST_TIME_ARGS (stream->samples[kf_index].timestamp));
    }
  } else {
    GST_DEBUG_OBJECT (qtdemux, "moving backwards to keyframe at %u (pts %"
        GST_TIME_FORMAT, kf_index,
        GST_TIME_ARGS (stream->samples[kf_index].timestamp));
    gst_qtdemux_move_stream (qtdemux, stream, kf_index);
  }

  return TRUE;
}

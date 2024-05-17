gst_qtdemux_perform_seek (GstQTDemux * qtdemux, GstSegment * segment)
{
  gint64 desired_offset;
  gint n;

  desired_offset = segment->last_stop;

  GST_DEBUG_OBJECT (qtdemux, "seeking to %" GST_TIME_FORMAT,
      GST_TIME_ARGS (desired_offset));

  if (segment->flags & GST_SEEK_FLAG_KEY_UNIT) {
    guint64 min_offset;

    min_offset = desired_offset;

     
    for (n = 0; n < qtdemux->n_streams; n++) {
      QtDemuxStream *str;
      guint32 index, kindex;
      guint32 seg_idx;
      guint64 media_start;
      guint64 media_time;
      guint64 seg_time;
      QtDemuxSegment *seg;

      str = qtdemux->streams[n];

      seg_idx = gst_qtdemux_find_segment (qtdemux, str, desired_offset);
      GST_DEBUG_OBJECT (qtdemux, "align segment %d", seg_idx);

       
      if (seg_idx == -1)
        continue;

       
      seg = &str->segments[seg_idx];
      seg_time = desired_offset - seg->time;

       
      media_start = seg->media_start + seg_time;

       
      index = gst_qtdemux_find_index (qtdemux, str, media_start);
      GST_DEBUG_OBJECT (qtdemux, "sample for %" GST_TIME_FORMAT " at %u",
          GST_TIME_ARGS (media_start), index);

       
      kindex = gst_qtdemux_find_keyframe (qtdemux, str, index);

       
      if (index != kindex) {
        index = kindex;

         
        media_time = str->samples[kindex].timestamp;
        GST_DEBUG_OBJECT (qtdemux, "keyframe at %u with time %" GST_TIME_FORMAT,
            kindex, GST_TIME_ARGS (media_time));

         
        if (media_time >= seg->media_start) {
          guint64 seg_time;

           
          seg_time = (media_time - seg->media_start) + seg->time;
          if (seg_time < min_offset)
            min_offset = seg_time;
        }
      }
    }
    GST_DEBUG_OBJECT (qtdemux, "keyframe seek, align to %"
        GST_TIME_FORMAT, GST_TIME_ARGS (desired_offset));
    desired_offset = min_offset;
  }

   
  for (n = 0; n < qtdemux->n_streams; n++) {
    QtDemuxStream *stream = qtdemux->streams[n];

    stream->time_position = desired_offset;
    stream->sample_index = -1;
    stream->segment_index = -1;
    stream->last_ret = GST_FLOW_OK;
  }
  segment->last_stop = desired_offset;
  segment->time = desired_offset;

   
  if (segment->stop == -1)
    segment->stop = segment->duration;

  return TRUE;
}

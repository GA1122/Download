gst_qtdemux_advance_sample (GstQTDemux * qtdemux, QtDemuxStream * stream)
{
  QtDemuxSample *sample;
  QtDemuxSegment *segment;

  if (stream->sample_index >= stream->to_sample) {
     
    GST_DEBUG_OBJECT (qtdemux, "reached max allowed sample %u, mark EOS",
        stream->to_sample);
    stream->time_position = -1;
    return;
  }

   
  stream->sample_index++;

   
  segment = &stream->segments[stream->segment_index];

   
  if (stream->sample_index >= stream->n_samples)
    goto next_segment;

   
  sample = &stream->samples[stream->sample_index];

   
  if (sample->timestamp >= segment->media_stop)
    goto next_segment;

  if (sample->timestamp >= segment->media_start) {
     
    stream->time_position =
        (sample->timestamp - segment->media_start) + segment->time;
  } else {
     
    stream->time_position = segment->time;
  }
  return;

   
next_segment:
  {
    GST_DEBUG_OBJECT (qtdemux, "segment %d ended ", stream->segment_index);

    if (stream->segment_index == stream->n_segments - 1) {
       
      stream->time_position = -1;
    } else {
       
      stream->time_position = segment->stop_time;
    }
     
    stream->segment_index = -1;
  }
}

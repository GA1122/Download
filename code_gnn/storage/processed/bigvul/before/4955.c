gst_qtdemux_prepare_current_sample (GstQTDemux * qtdemux,
    QtDemuxStream * stream, guint64 * offset, guint * size, guint64 * timestamp,
    guint64 * duration, gboolean * keyframe)
{
  QtDemuxSample *sample;
  guint64 time_position;
  guint32 seg_idx;

  g_return_val_if_fail (stream != NULL, FALSE);

  time_position = stream->time_position;
  if (time_position == -1)
    goto eos;

  seg_idx = stream->segment_index;
  if (seg_idx == -1) {
     
    seg_idx = gst_qtdemux_find_segment (qtdemux, stream, time_position);

     
    if (seg_idx == -1)
      goto eos;
  }

   
  if (stream->segment_index != seg_idx)
    gst_qtdemux_activate_segment (qtdemux, stream, seg_idx, time_position);

  GST_LOG_OBJECT (qtdemux, "segment active, index = %u of %u",
      stream->sample_index, stream->n_samples);

   
  while (stream->buffers) {
    GstBuffer *buffer = (GstBuffer *) stream->buffers->data;

    if (stream->discont) {
      GST_LOG_OBJECT (qtdemux, "marking discont buffer");
      GST_BUFFER_FLAG_SET (buffer, GST_BUFFER_FLAG_DISCONT);
      stream->discont = FALSE;
    }
    gst_buffer_set_caps (buffer, stream->caps);

    gst_pad_push (stream->pad, buffer);

    stream->buffers = g_slist_delete_link (stream->buffers, stream->buffers);
  }

  if (stream->sample_index >= stream->n_samples)
    goto eos;

   
  sample = &stream->samples[stream->sample_index];

  *timestamp = sample->timestamp + sample->pts_offset;
  *offset = sample->offset;
  *size = sample->size;
  *duration = sample->duration;
  *keyframe = stream->all_keyframe || sample->keyframe;

   
  if (stream->padding) {
    *offset += stream->padding;
    *size -= stream->padding;
  }

  return TRUE;

   
eos:
  {
    stream->time_position = -1;
    return FALSE;
  }
}

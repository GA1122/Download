gst_vmnc_dec_sink_event (GstVideoDecoder * bdec, GstEvent * event)
{
  const GstSegment *segment;

  if (GST_EVENT_TYPE (event) != GST_EVENT_SEGMENT)
    goto done;

  gst_event_parse_segment (event, &segment);

  if (segment->format == GST_FORMAT_TIME)
    gst_video_decoder_set_packetized (bdec, TRUE);
  else
    gst_video_decoder_set_packetized (bdec, FALSE);

done:
  return GST_VIDEO_DECODER_CLASS (gst_vmnc_dec_parent_class)->sink_event (bdec,
      event);
}

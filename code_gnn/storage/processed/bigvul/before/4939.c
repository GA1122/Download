gst_qtdemux_find_keyframe (GstQTDemux * qtdemux, QtDemuxStream * str,
    guint32 index)
{
  guint32 new_index = index;

  if (index >= str->n_samples) {
    new_index = str->n_samples;
    goto beach;
  }

   
  if (str->all_keyframe) {
    new_index = index;
    goto beach;
  }

   
  while (TRUE) {
    if (str->samples[new_index].keyframe)
      break;

    if (new_index == 0)
      break;

    new_index--;
  }

beach:
  GST_DEBUG_OBJECT (qtdemux, "searching for keyframe index before index %u "
      "gave %u", index, new_index);

  return new_index;
}

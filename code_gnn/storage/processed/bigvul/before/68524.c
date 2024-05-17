gst_asf_demux_add_audio_stream (GstASFDemux * demux,
    asf_stream_audio * audio, guint16 id, guint8 ** p_data, guint64 * p_size)
{
  GstTagList *tags = NULL;
  GstBuffer *extradata = NULL;
  GstPad *src_pad;
  GstCaps *caps;
  guint16 size_left = 0;
  gchar *codec_name = NULL;
  gchar *name = NULL;

  size_left = audio->size;

   
  name = g_strdup_printf ("audio_%u", demux->num_audio_streams);

  src_pad = gst_pad_new_from_static_template (&audio_src_template, name);
  g_free (name);

   
  if (size_left) {
    GST_INFO_OBJECT (demux, "Audio header contains %d bytes of "
        "codec specific data", size_left);

    g_assert (size_left <= *p_size);
    gst_asf_demux_get_buffer (&extradata, size_left, p_data, p_size);
  }

   
   
  caps = gst_riff_create_audio_caps (audio->codec_tag, NULL,
      (gst_riff_strf_auds *) audio, extradata, NULL, &codec_name, NULL);

  if (caps == NULL) {
    caps = gst_caps_new_simple ("audio/x-asf-unknown", "codec_id",
        G_TYPE_INT, (gint) audio->codec_tag, NULL);
  }

   
  if (codec_name) {
    tags = gst_tag_list_new (GST_TAG_AUDIO_CODEC, codec_name, NULL);
    g_free (codec_name);
  }

  if (audio->byte_rate > 0) {
     

    guint bitrate = audio->byte_rate * 8;

    if (tags == NULL)
      tags = gst_tag_list_new_empty ();

     
    gst_tag_list_add (tags, GST_TAG_MERGE_KEEP, GST_TAG_BITRATE, bitrate, NULL);
  }

  if (extradata)
    gst_buffer_unref (extradata);

  GST_INFO ("Adding audio stream #%u, id %u codec %u (0x%04x), tags=%"
      GST_PTR_FORMAT, demux->num_audio_streams, id, audio->codec_tag,
      audio->codec_tag, tags);

  ++demux->num_audio_streams;

  return gst_asf_demux_setup_pad (demux, src_pad, caps, id, FALSE, NULL, tags);
}

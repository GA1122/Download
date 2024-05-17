gst_asf_demux_process_header_ext (GstASFDemux * demux, guint8 * data,
    guint64 size)
{
  GstFlowReturn ret = GST_FLOW_OK;
  guint64 hdr_size;

   
  if (size < (16 + 2 + 4))
    goto not_enough_data;

   
  gst_asf_demux_skip_bytes (16 + 2, &data, &size);
  hdr_size = gst_asf_demux_get_uint32 (&data, &size);

  GST_INFO ("extended header object with a size of %u bytes", (guint) size);

   
  if (hdr_size > size)
    goto not_enough_data;

  while (hdr_size > 0) {
    ret = gst_asf_demux_process_object (demux, &data, &hdr_size);
    if (ret != GST_FLOW_OK)
      break;
  }

  return ret;

not_enough_data:
  {
    GST_ELEMENT_ERROR (demux, STREAM, DEMUX, (NULL),
        ("short read parsing extended header object"));
    return GST_FLOW_ERROR;
  }
}

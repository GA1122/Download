gst_asf_demux_update_caps_from_payload (GstASFDemux * demux, AsfStream * stream)
{
   
  GstTypeFindProbability prob = GST_TYPE_FIND_NONE;
  GstCaps *caps = NULL;
  int i;
  GstAdapter *adapter = gst_adapter_new ();

  for (i = 0; i < stream->payloads->len && prob < GST_TYPE_FIND_LIKELY; ++i) {
    const guint8 *data;
    AsfPayload *payload;
    int len;

    payload = &g_array_index (stream->payloads, AsfPayload, i);
    gst_adapter_push (adapter, gst_buffer_ref (payload->buf));
    len = gst_adapter_available (adapter);
    data = gst_adapter_map (adapter, len);

  again:

#define MIN_LENGTH 128

     
    while (TRUE) {
      if (len < MIN_LENGTH ||    
          (data[0] == 0x0b && data[1] == 0x77) ||        
          (data[0] == 0xFF && ((data[1] & 0xF0) >> 4) == 0xF))   
        break;
      ++data;
      --len;
    }

    gst_caps_take (&caps, gst_type_find_helper_for_data (GST_OBJECT (demux),
            data, len, &prob));

    if (prob < GST_TYPE_FIND_LIKELY) {
      ++data;
      --len;
      if (len > MIN_LENGTH)
         
        goto again;
    }

    gst_adapter_unmap (adapter);
  }

  gst_object_unref (adapter);

  if (caps) {
    gst_caps_take (&stream->caps, caps);
    return TRUE;
  } else {
    return FALSE;
  }
}

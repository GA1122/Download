qtdemux_parse_moov (GstQTDemux * qtdemux, guint8 * buffer, int length)
{
  GNode *cmov;

  qtdemux->moov_node = g_node_new (buffer);

  GST_DEBUG_OBJECT (qtdemux, "parsing 'moov' atom");
  qtdemux_parse_node (qtdemux, qtdemux->moov_node, buffer, length);

  cmov = qtdemux_tree_get_child_by_type (qtdemux->moov_node, FOURCC_cmov);
  if (cmov) {
    guint32 method;
    GNode *dcom;
    GNode *cmvd;

    dcom = qtdemux_tree_get_child_by_type (cmov, FOURCC_dcom);
    cmvd = qtdemux_tree_get_child_by_type (cmov, FOURCC_cmvd);
    if (dcom == NULL || cmvd == NULL)
      goto invalid_compression;

    method = QT_FOURCC ((guint8 *) dcom->data + 8);
    switch (method) {
#ifdef HAVE_ZLIB
      case GST_MAKE_FOURCC ('z', 'l', 'i', 'b'):{
        int uncompressed_length;
        int compressed_length;
        guint8 *buf;

        uncompressed_length = QT_UINT32 ((guint8 *) cmvd->data + 8);
        compressed_length = QT_UINT32 ((guint8 *) cmvd->data + 4) - 12;
        GST_LOG ("length = %d", uncompressed_length);

        buf =
            (guint8 *) qtdemux_inflate ((guint8 *) cmvd->data + 12,
            compressed_length, uncompressed_length);

        qtdemux->moov_node_compressed = qtdemux->moov_node;
        qtdemux->moov_node = g_node_new (buf);

        qtdemux_parse_node (qtdemux, qtdemux->moov_node, buf,
            uncompressed_length);
        break;
      }
#endif  
      default:
        GST_WARNING_OBJECT (qtdemux, "unknown or unhandled header compression "
            "type %" GST_FOURCC_FORMAT, GST_FOURCC_ARGS (method));
        break;
    }
  }
  return TRUE;

   
invalid_compression:
  {
    GST_ERROR_OBJECT (qtdemux, "invalid compressed header");
    return FALSE;
  }
}
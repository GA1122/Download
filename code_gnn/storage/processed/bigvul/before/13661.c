plugin_init (GstPlugin * plugin)
{
  if (!gst_element_register (plugin, "vmncdec", GST_RANK_PRIMARY,
          GST_TYPE_VMNC_DEC))
    return FALSE;
  return TRUE;
}

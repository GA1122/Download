gst_vmnc_dec_reset (GstVideoDecoder * decoder)
{
  GstVMncDec *dec = GST_VMNC_DEC (decoder);

  g_free (dec->imagedata);
  dec->imagedata = NULL;

  g_free (dec->cursor.cursordata);
  dec->cursor.cursordata = NULL;

  g_free (dec->cursor.cursormask);
  dec->cursor.cursormask = NULL;

  dec->cursor.visible = 0;

  dec->have_format = FALSE;

  if (dec->input_state)
    gst_video_codec_state_unref (dec->input_state);
  dec->input_state = NULL;

  return TRUE;
}

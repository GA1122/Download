static int GetCharacter(j_decompress_ptr jpeg_info)
{
  if (jpeg_info->src->bytes_in_buffer == 0)
    {
      (void) (*jpeg_info->src->fill_input_buffer)(jpeg_info);
      if (jpeg_info->err->msg_code == JWRN_JPEG_EOF)
        return EOF;
    }
  jpeg_info->src->bytes_in_buffer--;
  return((int) GETJOCTET(*jpeg_info->src->next_input_byte++));
}
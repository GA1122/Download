int dmarc_store_data(header_line *hdr) {
   
  if (dmarc_disable_verify != TRUE)
    from_header = hdr;
  return OK;
}

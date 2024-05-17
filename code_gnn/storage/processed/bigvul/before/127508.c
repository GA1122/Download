void term_source(j_decompress_ptr jd)
{
    decoder_source_mgr *src = (decoder_source_mgr *)jd->src;
    src->decoder->decoder()->jpegComplete();
}

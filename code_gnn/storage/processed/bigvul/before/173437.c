void omx_vdec::extract_demux_addr_offsets(OMX_BUFFERHEADERTYPE *buf_hdr)
{
    OMX_U32 bytes_to_parse = buf_hdr->nFilledLen;
    OMX_U8 *buf = buf_hdr->pBuffer + buf_hdr->nOffset;
    OMX_U32 index = 0;

    m_demux_entries = 0;

 while (index < bytes_to_parse) {
 if ( ((buf[index] == 0x00) && (buf[index+1] == 0x00) &&
 (buf[index+2] == 0x00) && (buf[index+3] == 0x01)) ||
 ((buf[index] == 0x00) && (buf[index+1] == 0x00) &&
 (buf[index+2] == 0x01)) ) {
            insert_demux_addr_offset(index);
 if (buf[index+2] == 0x01)  
                index += 3;
 else  
                index += 4;
 } else
            index++;
 }
    DEBUG_PRINT_LOW("Extracted (%u) demux entry offsets", (unsigned int)m_demux_entries);
 return;
}

int32_t MyOggExtractor::getPacketBlockSize(MediaBuffer *buffer) {
 const uint8_t *data =
 (const uint8_t *)buffer->data() + buffer->range_offset();

 size_t size = buffer->range_length();

    ogg_buffer buf;
    buf.data = (uint8_t *)data;
    buf.size = size;
    buf.refcount = 1;
    buf.ptr.owner = NULL;

    ogg_reference ref;
    ref.buffer = &buf;
    ref.begin = 0;
    ref.length = size;
    ref.next = NULL;

    ogg_packet pack;
    pack.packet = &ref;
    pack.bytes = ref.length;
    pack.b_o_s = 0;
    pack.e_o_s = 0;
    pack.granulepos = 0;
    pack.packetno = 0;

 return vorbis_packet_blocksize(&mVi, &pack);
}

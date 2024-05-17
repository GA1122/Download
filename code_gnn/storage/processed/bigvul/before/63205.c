void ff_amf_write_object_end(uint8_t **dst)
{
     
    bytestream_put_be24(dst, AMF_DATA_TYPE_OBJECT_END);
}

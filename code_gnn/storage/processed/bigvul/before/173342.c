zlib_rc(struct zlib *zlib)
  
{
 switch (zlib->rc)
 {
 case Z_OK: return "Z_OK";
 case Z_STREAM_END: return "Z_STREAM_END";
 case Z_NEED_DICT: return "Z_NEED_DICT";
 case Z_ERRNO: return "Z_ERRNO";
 case Z_STREAM_ERROR: return "Z_STREAM_ERROR";
 case Z_DATA_ERROR: return "Z_DATA_ERROR";
 case Z_MEM_ERROR: return "Z_MEM_ERROR";
 case Z_BUF_ERROR: return "Z_BUF_ERROR";
 case Z_VERSION_ERROR: return "Z_VERSION_ERROR";
 default: return "Z_*INVALID_RC*";
 }
}

  IW_IMPL(unsigned int) iw_get_ui32le(const iw_byte *b)
  {
	return b[0] | (b[1]<<8) | (b[2]<<16) | (b[3]<<24);
// 	return (unsigned int)b[0] | ((unsigned int)b[1]<<8) |
// 		((unsigned int)b[2]<<16) | ((unsigned int)b[3]<<24);
  }
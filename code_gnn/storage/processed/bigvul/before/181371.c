  IW_IMPL(unsigned int) iw_get_ui32be(const iw_byte *b)
  {
	return (b[0]<<24) | (b[1]<<16) | (b[2]<<8) | b[3];
// 	return ((unsigned int)b[0]<<24) | ((unsigned int)b[1]<<16) |
// 		((unsigned int)b[2]<<8) | (unsigned int)b[3];
  }
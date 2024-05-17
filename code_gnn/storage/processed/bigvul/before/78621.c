static int get_keylen(unsigned int alg_id, size_t *size)
{
	switch(alg_id) {
	case 0x01: *size = 192/8;  
		break;
	case 0x00:
	case 0x03: *size = 192/8;
		break;
	case 0x08: *size = 128/8;
		break;
	case 0x0A: *size = 192/8;
		break;
	case 0x0C: *size = 256/8;
		break;
	default:
		return SC_ERROR_INVALID_ARGUMENTS;
	}
	return SC_SUCCESS;
}
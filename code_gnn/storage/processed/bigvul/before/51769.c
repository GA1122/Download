parse_single_hex_dump_line(char* rec, guint8 *buf, guint byte_offset)
{
	int num_items_scanned;
	guint8 character;
	guint8 byte;


	for (num_items_scanned = 0; num_items_scanned < 16; num_items_scanned++) {
		character = *rec++;
		if (character >= '0' && character <= '9')
			byte = character - '0' + 0;
		else if (character >= 'A' && character <= 'F')
			byte = character - 'A' + 0xA;
		else if (character >= 'a' && character <= 'f')
			byte = character - 'a' + 0xa;
		else if (character == ' ' || character == '\r' || character == '\n' || character == '\0') {
			 
			break;
		} else
			return -1;  
		byte <<= 4;
		character = *rec++ & 0xFF;
		if (character >= '0' && character <= '9')
			byte += character - '0' + 0;
		else if (character >= 'A' && character <= 'F')
			byte += character - 'A' + 0xA;
		else if (character >= 'a' && character <= 'f')
			byte += character - 'a' + 0xa;
		else
			return -1;  
		buf[byte_offset + num_items_scanned] = byte;
		character = *rec++ & 0xFF;
		if (character == '\0' || character == '\r' || character == '\n') {
			 
			break;
		} else if (character != ' ') {
			 
			return -1;
		}
	}
	if (num_items_scanned == 0)
		return -1;

	return num_items_scanned;
}

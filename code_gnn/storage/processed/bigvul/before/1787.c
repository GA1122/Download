int FAST_FUNC read_bunzip(bunzip_data *bd, char *outbuf, int len)
{
	const uint32_t *dbuf;
	int pos, current, previous;
	uint32_t CRC;

	 
	if (bd->writeCount < 0)
		return bd->writeCount;

	dbuf = bd->dbuf;

	 
	pos = bd->writePos;
	current = bd->writeCurrent;
	CRC = bd->writeCRC;  

	 
	if (bd->writeCopies) {

 dec_writeCopies:
		 
		--bd->writeCopies;

		 
		for (;;) {

			 
			if (--len < 0) {
				 
				goto outbuf_full;
			}

			 
			*outbuf++ = current;
			CRC = (CRC << 8) ^ bd->crc32Table[(CRC >> 24) ^ current];

			 
			if (bd->writeCopies) {
				 
				 
				 
				 
				goto dec_writeCopies;
			}
 decode_next_byte:
			if (--bd->writeCount < 0)
				break;  

			 
			previous = current;
			pos = dbuf[pos];
			current = (uint8_t)pos;
			pos >>= 8;

			 
			if (--bd->writeRunCountdown != 0) {
				if (current != previous)
					bd->writeRunCountdown = 4;
			} else {
				 
				 
				bd->writeCopies = current;
				current = previous;
				bd->writeRunCountdown = 5;

				 
				if (!bd->writeCopies) goto decode_next_byte;

				 
				--bd->writeCopies;
			}
		}  

		 
		bd->writeCRC = CRC = ~CRC;
		bd->totalCRC = ((bd->totalCRC << 1) | (bd->totalCRC >> 31)) ^ CRC;

		 
		if (CRC != bd->headerCRC) {
			bd->totalCRC = bd->headerCRC + 1;
			return RETVAL_LAST_BLOCK;
		}
	}

	 
	{
		int r = get_next_block(bd);
		if (r) {  
			bd->writeCount = r;
			return (r != RETVAL_LAST_BLOCK) ? r : len;
		}
	}

	CRC = ~0;
	pos = bd->writePos;
	current = bd->writeCurrent;
	goto decode_next_byte;

 outbuf_full:
	 
	bd->writePos = pos;
	bd->writeCurrent = current;
	bd->writeCRC = CRC;

	bd->writeCopies++;

	return 0;
}

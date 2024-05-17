lha_read_file_extended_header(struct archive_read *a, struct lha *lha,
    uint16_t *crc, int sizefield_length, size_t limitsize, size_t *total_size)
{
	const void *h;
	const unsigned char *extdheader;
	size_t	extdsize;
	size_t	datasize;
	unsigned int i;
	unsigned char extdtype;

#define EXT_HEADER_CRC		0x00		 
#define EXT_FILENAME		0x01		 
#define EXT_DIRECTORY		0x02		 
#define EXT_DOS_ATTR		0x40		 
#define EXT_TIMESTAMP		0x41		 
#define EXT_FILESIZE		0x42		 
#define EXT_TIMEZONE		0x43		 
#define EXT_UTF16_FILENAME	0x44		 
#define EXT_UTF16_DIRECTORY	0x45		 
#define EXT_CODEPAGE		0x46		 
#define EXT_UNIX_MODE		0x50		 
#define EXT_UNIX_GID_UID	0x51		 
#define EXT_UNIX_GNAME		0x52		 
#define EXT_UNIX_UNAME		0x53		 
#define EXT_UNIX_MTIME		0x54		 
#define EXT_OS2_NEW_ATTR	0x7f		 
#define EXT_NEW_ATTR		0xff		 

	*total_size = sizefield_length;

	for (;;) {
		 
		if ((h =
		    __archive_read_ahead(a, sizefield_length, NULL)) == NULL)
			return (truncated_error(a));
		 
		if (sizefield_length == sizeof(uint16_t))
			extdsize = archive_le16dec(h);
		else
			extdsize = archive_le32dec(h);
		if (extdsize == 0) {
			 
			if (crc != NULL)
				*crc = lha_crc16(*crc, h, sizefield_length);
			__archive_read_consume(a, sizefield_length);
			return (ARCHIVE_OK);
		}

		 
		if (((uint64_t)*total_size + extdsize) >
				    (uint64_t)limitsize ||
		    extdsize <= (size_t)sizefield_length)
			goto invalid;

		 
		if ((h = __archive_read_ahead(a, extdsize, NULL)) == NULL)
			return (truncated_error(a));
		*total_size += extdsize;

		extdheader = (const unsigned char *)h;
		 
		extdtype = extdheader[sizefield_length];
		 
		datasize = extdsize - (1 + sizefield_length);
		 
		extdheader += sizefield_length + 1;

		if (crc != NULL && extdtype != EXT_HEADER_CRC)
			*crc = lha_crc16(*crc, h, extdsize);
		switch (extdtype) {
		case EXT_HEADER_CRC:
			 
			if (datasize >= 2) {
				lha->header_crc = archive_le16dec(extdheader);
				if (crc != NULL) {
					static const char zeros[2] = {0, 0};
					*crc = lha_crc16(*crc, h,
					    extdsize - datasize);
					 
					*crc = lha_crc16(*crc, zeros, 2);
					*crc = lha_crc16(*crc,
					    extdheader+2, datasize - 2);
				}
			}
			break;
		case EXT_FILENAME:
			if (datasize == 0) {
				 
				archive_string_empty(&lha->filename);
				break;
			}
			if (extdheader[0] == '\0')
				goto invalid;
			archive_strncpy(&lha->filename,
			    (const char *)extdheader, datasize);
			break;
		case EXT_DIRECTORY:
			if (datasize == 0 || extdheader[0] == '\0')
				 
				goto invalid;

			archive_strncpy(&lha->dirname,
		  	    (const char *)extdheader, datasize);
			 
			for (i = 0; i < lha->dirname.length; i++) {
				if ((unsigned char)lha->dirname.s[i] == 0xFF)
					lha->dirname.s[i] = '/';
			}
			 
			if (lha->dirname.s[lha->dirname.length-1] != '/')
				 
				goto invalid;
			break;
		case EXT_DOS_ATTR:
			if (datasize == 2)
				lha->dos_attr = (unsigned char)
				    (archive_le16dec(extdheader) & 0xff);
			break;
		case EXT_TIMESTAMP:
			if (datasize == (sizeof(uint64_t) * 3)) {
				lha->birthtime = lha_win_time(
				    archive_le64dec(extdheader),
				    &lha->birthtime_tv_nsec);
				extdheader += sizeof(uint64_t);
				lha->mtime = lha_win_time(
				    archive_le64dec(extdheader),
				    &lha->mtime_tv_nsec);
				extdheader += sizeof(uint64_t);
				lha->atime = lha_win_time(
				    archive_le64dec(extdheader),
				    &lha->atime_tv_nsec);
				lha->setflag |= BIRTHTIME_IS_SET |
				    ATIME_IS_SET;
			}
			break;
		case EXT_FILESIZE:
			if (datasize == sizeof(uint64_t) * 2) {
				lha->compsize = archive_le64dec(extdheader);
				extdheader += sizeof(uint64_t);
				lha->origsize = archive_le64dec(extdheader);
			}
			break;
		case EXT_CODEPAGE:
			 
			if (datasize == sizeof(uint32_t)) {
				struct archive_string cp;
				const char *charset;

				archive_string_init(&cp);
				switch (archive_le32dec(extdheader)) {
				case 65001:  
					charset = "UTF-8";
					break;
				default:
					archive_string_sprintf(&cp, "CP%d",
					    (int)archive_le32dec(extdheader));
					charset = cp.s;
					break;
				}
				lha->sconv =
				    archive_string_conversion_from_charset(
					&(a->archive), charset, 1);
				archive_string_free(&cp);
				if (lha->sconv == NULL)
					return (ARCHIVE_FATAL);
			}
			break;
		case EXT_UNIX_MODE:
			if (datasize == sizeof(uint16_t)) {
				lha->mode = archive_le16dec(extdheader);
				lha->setflag |= UNIX_MODE_IS_SET;
			}
			break;
		case EXT_UNIX_GID_UID:
			if (datasize == (sizeof(uint16_t) * 2)) {
				lha->gid = archive_le16dec(extdheader);
				lha->uid = archive_le16dec(extdheader+2);
			}
			break;
		case EXT_UNIX_GNAME:
			if (datasize > 0)
				archive_strncpy(&lha->gname,
				    (const char *)extdheader, datasize);
			break;
		case EXT_UNIX_UNAME:
			if (datasize > 0)
				archive_strncpy(&lha->uname,
				    (const char *)extdheader, datasize);
			break;
		case EXT_UNIX_MTIME:
			if (datasize == sizeof(uint32_t))
				lha->mtime = archive_le32dec(extdheader);
			break;
		case EXT_OS2_NEW_ATTR:
			 
			if (datasize == 16) {
				lha->dos_attr = (unsigned char)
				    (archive_le16dec(extdheader) & 0xff);
				lha->mode = archive_le16dec(extdheader+2);
				lha->gid = archive_le16dec(extdheader+4);
				lha->uid = archive_le16dec(extdheader+6);
				lha->birthtime = archive_le32dec(extdheader+8);
				lha->atime = archive_le32dec(extdheader+12);
				lha->setflag |= UNIX_MODE_IS_SET
				    | BIRTHTIME_IS_SET | ATIME_IS_SET;
			}
			break;
		case EXT_NEW_ATTR:
			if (datasize == 20) {
				lha->mode = (mode_t)archive_le32dec(extdheader);
				lha->gid = archive_le32dec(extdheader+4);
				lha->uid = archive_le32dec(extdheader+8);
				lha->birthtime = archive_le32dec(extdheader+12);
				lha->atime = archive_le32dec(extdheader+16);
				lha->setflag |= UNIX_MODE_IS_SET
				    | BIRTHTIME_IS_SET | ATIME_IS_SET;
			}
			break;
		case EXT_TIMEZONE:		 
		case EXT_UTF16_FILENAME:	 
		case EXT_UTF16_DIRECTORY:	 
		default:
			break;
		}

		__archive_read_consume(a, extdsize);
	}
invalid:
	archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
	    "Invalid extended LHa header");
	return (ARCHIVE_FATAL);
}

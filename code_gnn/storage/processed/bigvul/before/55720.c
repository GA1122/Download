process_extra(const char *p, size_t extra_length, struct zip_entry* zip_entry)
{
	unsigned offset = 0;

	while (offset < extra_length - 4) {
		unsigned short headerid = archive_le16dec(p + offset);
		unsigned short datasize = archive_le16dec(p + offset + 2);

		offset += 4;
		if (offset + datasize > extra_length) {
			break;
		}
#ifdef DEBUG
		fprintf(stderr, "Header id 0x%04x, length %d\n",
		    headerid, datasize);
#endif
		switch (headerid) {
		case 0x0001:
			 
			zip_entry->flags |= LA_USED_ZIP64;
			if (zip_entry->uncompressed_size == 0xffffffff) {
				if (datasize < 8)
					break;
				zip_entry->uncompressed_size =
				    archive_le64dec(p + offset);
				offset += 8;
				datasize -= 8;
			}
			if (zip_entry->compressed_size == 0xffffffff) {
				if (datasize < 8)
					break;
				zip_entry->compressed_size =
				    archive_le64dec(p + offset);
				offset += 8;
				datasize -= 8;
			}
			if (zip_entry->local_header_offset == 0xffffffff) {
				if (datasize < 8)
					break;
				zip_entry->local_header_offset =
				    archive_le64dec(p + offset);
				offset += 8;
				datasize -= 8;
			}
			 
			break;
#ifdef DEBUG
		case 0x0017:
		{
			 
			if (archive_le16dec(p + offset) == 2) {
				unsigned algId =
					archive_le16dec(p + offset + 2);
				unsigned bitLen =
					archive_le16dec(p + offset + 4);
				int	 flags =
					archive_le16dec(p + offset + 6);
				fprintf(stderr, "algId=0x%04x, bitLen=%u, "
				    "flgas=%d\n", algId, bitLen,flags);
			}
			break;
		}
#endif
		case 0x5455:
		{
			 
			int flags = p[offset];
			offset++;
			datasize--;
			 
			if (flags & 0x01)
			{
#ifdef DEBUG
				fprintf(stderr, "mtime: %lld -> %d\n",
				    (long long)zip_entry->mtime,
				    archive_le32dec(p + offset));
#endif
				if (datasize < 4)
					break;
				zip_entry->mtime = archive_le32dec(p + offset);
				offset += 4;
				datasize -= 4;
			}
			if (flags & 0x02)
			{
				if (datasize < 4)
					break;
				zip_entry->atime = archive_le32dec(p + offset);
				offset += 4;
				datasize -= 4;
			}
			if (flags & 0x04)
			{
				if (datasize < 4)
					break;
				zip_entry->ctime = archive_le32dec(p + offset);
				offset += 4;
				datasize -= 4;
			}
			break;
		}
		case 0x5855:
		{
			 
			if (datasize >= 8) {
				zip_entry->atime = archive_le32dec(p + offset);
				zip_entry->mtime =
				    archive_le32dec(p + offset + 4);
			}
			if (datasize >= 12) {
				zip_entry->uid =
				    archive_le16dec(p + offset + 8);
				zip_entry->gid =
				    archive_le16dec(p + offset + 10);
			}
			break;
		}
		case 0x6c78:
		{
			 
			 
			int bitmap, bitmap_last;

			if (datasize < 1)
				break;
			bitmap_last = bitmap = 0xff & p[offset];
			offset += 1;
			datasize -= 1;

			 
			while ((bitmap_last & 0x80) != 0
			    && datasize >= 1) {
				bitmap_last = p[offset];
				offset += 1;
				datasize -= 1;
			}

			if (bitmap & 1) {
				 
				if (datasize < 2)
					break;
				zip_entry->system
				    = archive_le16dec(p + offset) >> 8;
				offset += 2;
				datasize -= 2;
			}
			if (bitmap & 2) {
				 
				uint32_t internal_attributes;
				if (datasize < 2)
					break;
				internal_attributes
				    = archive_le16dec(p + offset);
				 
				(void)internal_attributes;  
				offset += 2;
				datasize -= 2;
			}
			if (bitmap & 4) {
				 
				uint32_t external_attributes;
				if (datasize < 4)
					break;
				external_attributes
				    = archive_le32dec(p + offset);
				if (zip_entry->system == 3) {
					zip_entry->mode
					    = external_attributes >> 16;
				} else if (zip_entry->system == 0) {
					if (0x10 == (external_attributes & 0x10)) {
						zip_entry->mode = AE_IFDIR | 0775;
					} else {
						zip_entry->mode = AE_IFREG | 0664;
					}
					if (0x01 == (external_attributes & 0x01)) {
						zip_entry->mode &= 0555;
					}
				} else {
					zip_entry->mode = 0;
				}
				offset += 4;
				datasize -= 4;
			}
			if (bitmap & 8) {
				 
				uint32_t comment_length;
				if (datasize < 2)
					break;
				comment_length
				    = archive_le16dec(p + offset);
				offset += 2;
				datasize -= 2;

				if (datasize < comment_length)
					break;
				 
				offset += comment_length;
				datasize -= comment_length;
			}
			break;
		}
		case 0x7855:
			 
#ifdef DEBUG
			fprintf(stderr, "uid %d gid %d\n",
			    archive_le16dec(p + offset),
			    archive_le16dec(p + offset + 2));
#endif
			if (datasize >= 2)
				zip_entry->uid = archive_le16dec(p + offset);
			if (datasize >= 4)
				zip_entry->gid =
				    archive_le16dec(p + offset + 2);
			break;
		case 0x7875:
		{
			 
			int uidsize = 0, gidsize = 0;

			 
			if (datasize >= 1 && p[offset] == 1) { 
				if (datasize >= 4) {
					 
					uidsize = 0xff & (int)p[offset+1];
					if (uidsize == 2)
						zip_entry->uid =
						    archive_le16dec(
						        p + offset + 2);
					else if (uidsize == 4 && datasize >= 6)
						zip_entry->uid =
						    archive_le32dec(
						        p + offset + 2);
				}
				if (datasize >= (2 + uidsize + 3)) {
					 
					gidsize = 0xff & (int)p[offset+2+uidsize];
					if (gidsize == 2)
						zip_entry->gid =
						    archive_le16dec(
						        p+offset+2+uidsize+1);
					else if (gidsize == 4 &&
					    datasize >= (2 + uidsize + 5))
						zip_entry->gid =
						    archive_le32dec(
						        p+offset+2+uidsize+1);
				}
			}
			break;
		}
		case 0x9901:
			 
			if (p[offset + 2] == 'A' && p[offset + 3] == 'E') {
				 
				zip_entry->aes_extra.vendor =
				    archive_le16dec(p + offset);
				 
				zip_entry->aes_extra.strength = p[offset + 4];
				 
				zip_entry->aes_extra.compression =
				    p[offset + 5];
			}
			break;
		default:
			break;
		}
		offset += datasize;
	}
#ifdef DEBUG
	if (offset != extra_length)
	{
		fprintf(stderr,
		    "Extra data field contents do not match reported size!\n");
	}
#endif
}

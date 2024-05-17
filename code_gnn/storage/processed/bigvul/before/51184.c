archive_read_format_iso9660_bid(struct archive_read *a, int best_bid)
{
	struct iso9660 *iso9660;
	ssize_t bytes_read;
	const unsigned char *p;
	int seenTerminator;

	 
	if (best_bid > 48)
		return (-1);

	iso9660 = (struct iso9660 *)(a->format->data);

	 
#define RESERVED_AREA	(SYSTEM_AREA_BLOCK * LOGICAL_BLOCK_SIZE)
	p = __archive_read_ahead(a,
	    RESERVED_AREA + 8 * LOGICAL_BLOCK_SIZE,
	    &bytes_read);
	if (p == NULL)
	    return (-1);

	 
	bytes_read -= RESERVED_AREA;
	p += RESERVED_AREA;

	 
	seenTerminator = 0;
	for (; bytes_read > LOGICAL_BLOCK_SIZE;
	    bytes_read -= LOGICAL_BLOCK_SIZE, p += LOGICAL_BLOCK_SIZE) {
		 
		if (p[0] >= 4 && p[0] <= 254)
			return (0);
		 
		if (memcmp(p + 1, "CD001", 5) != 0)
			return (0);
		if (isPVD(iso9660, p))
			continue;
		if (!iso9660->joliet.location) {
			if (isJolietSVD(iso9660, p))
				continue;
		}
		if (isBootRecord(iso9660, p))
			continue;
		if (isEVD(iso9660, p))
			continue;
		if (isSVD(iso9660, p))
			continue;
		if (isVolumePartition(iso9660, p))
			continue;
		if (isVDSetTerminator(iso9660, p)) {
			seenTerminator = 1;
			break;
		}
		return (0);
	}
	 
	if (seenTerminator && iso9660->primary.location > 16)
		return (48);

	 
	return (0);
}

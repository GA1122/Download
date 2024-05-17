capture_pktap(const guchar *pd, int len, packet_counts *ld)
{
	guint32  hdrlen, rectype, dlt;

	hdrlen = pletoh32(pd);
	if (hdrlen < MIN_PKTAP_HDR_LEN || !BYTES_ARE_IN_FRAME(0, len, hdrlen)) {
		ld->other++;
		return;
	}

	rectype = pletoh32(pd+4);
	if (rectype != PKT_REC_PACKET) {
		ld->other++;
		return;
	}

	dlt = pletoh32(pd+4);

	 
	switch (dlt) {

	case 1:  
		capture_eth(pd, hdrlen, len, ld);
		return;

	default:
		break;
	}

	ld->other++;
}

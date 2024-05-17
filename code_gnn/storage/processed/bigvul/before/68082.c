static int check_bytes(const ut8 *buf, ut64 length) {
	if (!buf || length < 8) {
		return false;
	}
	if (!memcmp (buf, "dex\n035\0", 8)) {
		return true;
	}
	if (!memcmp (buf, "dex\n036\0", 8)) {
		return true;
	}
	if (!memcmp (buf, "dex\n009\0", 8)) {
		return true;
	}
	if (!memcmp (buf, "dex\n009\0", 8)) {
		return true;
	}
	if (!memcmp (buf, "dex\n", 4)) {
		return true;
	}
	return false;
}

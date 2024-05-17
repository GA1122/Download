static zend_always_inline int zend_mm_bitset_find_zero_and_set(zend_mm_bitset *bitset, int size)
{
	int i = 0;

	do {
		zend_mm_bitset tmp = bitset[i];
		if (tmp != (zend_mm_bitset)-1) {
			int n = zend_mm_bitset_nts(tmp);
			bitset[i] |= Z_UL(1) << n;
			return i * ZEND_MM_BITSET_LEN + n;
		}
		i++;
	} while (i < size);
	return -1;
}

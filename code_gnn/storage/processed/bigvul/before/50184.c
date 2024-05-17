static zend_always_inline int zend_mm_bitset_find_zero(zend_mm_bitset *bitset, int size)
{
	int i = 0;

	do {
		zend_mm_bitset tmp = bitset[i];
		if (tmp != (zend_mm_bitset)-1) {
			return i * ZEND_MM_BITSET_LEN + zend_mm_bitset_nts(tmp);
		}
		i++;
	} while (i < size);
	return -1;
}

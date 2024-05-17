static zend_always_inline int zend_mm_bitset_is_set(zend_mm_bitset *bitset, int bit)
{
	return (bitset[bit / ZEND_MM_BITSET_LEN] & (Z_L(1) << (bit & (ZEND_MM_BITSET_LEN-1)))) != 0;
}

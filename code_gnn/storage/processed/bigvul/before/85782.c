static int madvise_need_mmap_write(int behavior)
{
	switch (behavior) {
	case MADV_REMOVE:
	case MADV_WILLNEED:
	case MADV_DONTNEED:
	case MADV_FREE:
		return 0;
	default:
		 
		return 1;
	}
}
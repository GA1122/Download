static inline int user_space_fault(unsigned long trans_exc_code)
{
	 
	trans_exc_code &= 3;
	if (trans_exc_code == 2)
		 
		return current->thread.mm_segment.ar4;
	if (user_mode == HOME_SPACE_MODE)
		 
		return trans_exc_code == 3;
	 
	return trans_exc_code != 3;
}

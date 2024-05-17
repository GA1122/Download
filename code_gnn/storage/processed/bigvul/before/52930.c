static int qib_compatible_subctxts(int user_swmajor, int user_swminor)
{
	 
	if (QIB_USER_SWMAJOR != user_swmajor) {
		 
		return 0;
	}
	if (QIB_USER_SWMAJOR == 1) {
		switch (QIB_USER_SWMINOR) {
		case 0:
		case 1:
		case 2:
			 
			return 0;
		case 3:
			 
			return user_swminor == 3;
		default:
			 
			return user_swminor <= QIB_USER_SWMINOR;
		}
	}
	 
	return 0;
}

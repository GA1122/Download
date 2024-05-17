static BOOL rectangle_contained_in_band(const RECTANGLE_16* band, const RECTANGLE_16* endPtr,
                                        const RECTANGLE_16* rect)
{
	UINT16 refY = band->top;

	if ((band->top > rect->top) || (rect->bottom > band->bottom))
		return FALSE;

	 
	while ((band < endPtr) && (band->top == refY) && (band->left <= rect->left))
	{
		if (rect->right <= band->right)
			return TRUE;

		band++;
	}

	return FALSE;
}

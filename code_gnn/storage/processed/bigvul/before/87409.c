static void region16_copy_band_with_union(RECTANGLE_16* dst,
        const RECTANGLE_16* src, const RECTANGLE_16* end,
        UINT16 newTop, UINT16 newBottom,
        const RECTANGLE_16* unionRect,
        UINT32* dstCounter,
        const RECTANGLE_16** srcPtr, RECTANGLE_16** dstPtr)
{
	UINT16 refY = src->top;
	const RECTANGLE_16* startOverlap, *endOverlap;

	 

	if (unionRect)
	{
		 
		while ((src < end) && (src->top == refY) && (src->right < unionRect->left))
		{
			dst->top = newTop;
			dst->bottom = newBottom;
			dst->right = src->right;
			dst->left = src->left;
			src++;
			dst++;
			*dstCounter += 1;
		}

		 
		startOverlap = unionRect;
		endOverlap = unionRect;

		if ((src < end) && (src->top == refY) && (src->left < unionRect->left))
			startOverlap = src;

		while ((src < end) && (src->top == refY) && (src->right < unionRect->right))
		{
			src++;
		}

		if ((src < end) && (src->top == refY) && (src->left < unionRect->right))
		{
			endOverlap = src;
			src++;
		}

		dst->bottom = newBottom;
		dst->top = newTop;
		dst->left = startOverlap->left;
		dst->right = endOverlap->right;
		dst++;
		*dstCounter += 1;
	}

	 
	while ((src < end) && (src->top == refY))
	{
		dst->top = newTop;
		dst->bottom = newBottom;
		dst->right = src->right;
		dst->left = src->left;
		src++;
		dst++;
		*dstCounter += 1;
	}

	if (srcPtr)
		*srcPtr = src;

	*dstPtr = dst;
}

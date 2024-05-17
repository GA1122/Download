static int appendDynamic(dynamicPtr * dp, const void *src, int size)
{
	int bytesNeeded;
	char *tmp;

	if(!dp->dataGood) {
		return FALSE;
	}

	 
	bytesNeeded = dp->pos + size;

	if(bytesNeeded > dp->realSize) {
		 
		if(!dp->freeOK) {
			return FALSE;
		}

		if(overflow2(dp->realSize, 2)) {
			return FALSE;
		}

		if(!gdReallocDynamic(dp, bytesNeeded * 2)) {
			dp->dataGood = FALSE;
			return FALSE;
		}
	}

	 

	 

	tmp = (char *)dp->data;
	memcpy ((void *)(tmp + (dp->pos)), src, size);
	dp->pos += size;

	if(dp->pos > dp->logicalSize) {
		dp->logicalSize = dp->pos;
	};

	return TRUE;
}

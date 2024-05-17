static int gdReallocDynamic(dynamicPtr *dp, int required)
{
	void *newPtr;

	 
	if((newPtr = gdRealloc(dp->data, required))) {
		dp->realSize = required;
		dp->data = newPtr;
		return TRUE;
	}

	 
	newPtr = gdMalloc(required);
	if(!newPtr) {
		dp->dataGood = FALSE;
		return FALSE;
	}

	 
	memcpy(newPtr, dp->data, dp->logicalSize);
	gdFree(dp->data);
	dp->data = newPtr;

	dp->realSize = required;
	return TRUE;
}

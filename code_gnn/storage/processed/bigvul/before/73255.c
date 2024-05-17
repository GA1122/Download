static int trimDynamic(dynamicPtr *dp)
{
	 
	if(!dp->freeOK) {
		return TRUE;
	}

	return gdReallocDynamic(dp, dp->logicalSize);
}

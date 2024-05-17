static int dynamicSeek(struct gdIOCtx *ctx, const int pos)
{
	int bytesNeeded;
	dynamicPtr *dp;
	dpIOCtx *dctx;

	dctx = (dpIOCtx *)ctx;
	dp = dctx->dp;

	if(!dp->dataGood) {
		return FALSE;
	}

	bytesNeeded = pos;
	if(bytesNeeded > dp->realSize) {
		 
		if(!dp->freeOK) {
			return FALSE;
		}

		if(overflow2(dp->realSize, 2)) {
			return FALSE;
		}

		if(!gdReallocDynamic(dp, dp->realSize * 2)) {
			dp->dataGood = FALSE;
			return FALSE;
		}
	}

	 

	 
	if(pos > dp->logicalSize) {
		dp->logicalSize = pos;
	};

	dp->pos = pos;

	return TRUE;
}

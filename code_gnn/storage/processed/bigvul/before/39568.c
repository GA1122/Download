BOOL license_read_product_info(wStream* s, PRODUCT_INFO* productInfo)
{
	if (Stream_GetRemainingLength(s) < 8)
		return FALSE;

	Stream_Read_UINT32(s, productInfo->dwVersion);  

	Stream_Read_UINT32(s, productInfo->cbCompanyName);  

	if (Stream_GetRemainingLength(s) < productInfo->cbCompanyName + 4)
		return FALSE;

	productInfo->pbCompanyName = (BYTE*) malloc(productInfo->cbCompanyName);
	Stream_Read(s, productInfo->pbCompanyName, productInfo->cbCompanyName);

	Stream_Read_UINT32(s, productInfo->cbProductId);  

	if (Stream_GetRemainingLength(s) < productInfo->cbProductId)
	{
		free(productInfo->pbCompanyName);
		productInfo->pbCompanyName = NULL;
		return FALSE;
	}

	productInfo->pbProductId = (BYTE*) malloc(productInfo->cbProductId);
	Stream_Read(s, productInfo->pbProductId, productInfo->cbProductId);

	return TRUE;
}

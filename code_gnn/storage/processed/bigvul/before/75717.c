		const URI_TYPE(Uri) * source, UriMemoryManager * memory) {
	 
	
	 
	dest->userInfo = source->userInfo;

	 
	dest->hostText = source->hostText;

	 
	if (source->hostData.ip4 != NULL) {
		dest->hostData.ip4 = memory->malloc(memory, sizeof(UriIp4));
		if (dest->hostData.ip4 == NULL) {
			return URI_FALSE;  
		}
		*(dest->hostData.ip4) = *(source->hostData.ip4);
		dest->hostData.ip6 = NULL;
		dest->hostData.ipFuture.first = NULL;
		dest->hostData.ipFuture.afterLast = NULL;
	} else if (source->hostData.ip6 != NULL) {
		dest->hostData.ip4 = NULL;
		dest->hostData.ip6 = memory->malloc(memory, sizeof(UriIp6));
		if (dest->hostData.ip6 == NULL) {
			return URI_FALSE;  
		}
		*(dest->hostData.ip6) = *(source->hostData.ip6);
		dest->hostData.ipFuture.first = NULL;
		dest->hostData.ipFuture.afterLast = NULL;
	} else {
		dest->hostData.ip4 = NULL;
		dest->hostData.ip6 = NULL;
		dest->hostData.ipFuture = source->hostData.ipFuture;
	}

	 
	dest->portText = source->portText;

	return URI_TRUE;
}

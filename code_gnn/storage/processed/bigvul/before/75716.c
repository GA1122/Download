		const URI_TYPE(Uri) * source, UriMemoryManager * memory) {
	if (source->pathHead == NULL) {
		 
		dest->pathHead = NULL;
		dest->pathTail = NULL;
	} else {
		 
		URI_TYPE(PathSegment) * sourceWalker = source->pathHead;
		URI_TYPE(PathSegment) * destPrev = NULL;
		do {
			URI_TYPE(PathSegment) * cur = memory->malloc(memory, sizeof(URI_TYPE(PathSegment)));
			if (cur == NULL) {
				 
				if (destPrev != NULL) {
					destPrev->next = NULL;
				}
				return URI_FALSE;  
			}

			 
			cur->text = sourceWalker->text;
			if (destPrev == NULL) {
				 
				dest->pathHead = cur;
			} else {
				destPrev->next = cur;
			}
			destPrev = cur;
			sourceWalker = sourceWalker->next;
		} while (sourceWalker != NULL);
		dest->pathTail = destPrev;
		dest->pathTail->next = NULL;
	}

	dest->absolutePath = source->absolutePath;
	return URI_TRUE;
}

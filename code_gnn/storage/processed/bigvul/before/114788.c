__xmlBufferAllocScheme(void) {
    if (IS_MAIN_THREAD)
	return (&xmlBufferAllocScheme);
    else
	return (&xmlGetGlobalState()->xmlBufferAllocScheme);
}

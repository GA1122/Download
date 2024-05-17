__xmlParserInputBufferCreateFilenameValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlParserInputBufferCreateFilenameValue);
    else
	return (&xmlGetGlobalState()->xmlParserInputBufferCreateFilenameValue);
}

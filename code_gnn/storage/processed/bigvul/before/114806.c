__xmlParserDebugEntities(void) {
    if (IS_MAIN_THREAD)
	return (&xmlParserDebugEntities);
    else
	return (&xmlGetGlobalState()->xmlParserDebugEntities);
}

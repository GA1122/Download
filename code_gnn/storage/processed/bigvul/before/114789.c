__xmlDefaultSAXHandler(void) {
    if (IS_MAIN_THREAD)
	return (&xmlDefaultSAXHandler);
    else
	return (&xmlGetGlobalState()->xmlDefaultSAXHandler);
}

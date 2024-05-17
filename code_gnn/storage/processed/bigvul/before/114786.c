__htmlDefaultSAXHandler(void) {
    if (IS_MAIN_THREAD)
	return (&htmlDefaultSAXHandler);
    else
	return (&xmlGetGlobalState()->htmlDefaultSAXHandler);
}

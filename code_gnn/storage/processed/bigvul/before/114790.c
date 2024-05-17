__xmlDefaultSAXLocator(void) {
    if (IS_MAIN_THREAD)
	return (&xmlDefaultSAXLocator);
    else
	return (&xmlGetGlobalState()->xmlDefaultSAXLocator);
}

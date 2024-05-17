__xmlStructuredError(void) {
    if (IS_MAIN_THREAD)
	return (&xmlStructuredError);
    else
	return (&xmlGetGlobalState()->xmlStructuredError);
}

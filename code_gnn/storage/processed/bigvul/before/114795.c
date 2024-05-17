__xmlGenericErrorContext(void) {
    if (IS_MAIN_THREAD)
	return (&xmlGenericErrorContext);
    else
	return (&xmlGetGlobalState()->xmlGenericErrorContext);
}

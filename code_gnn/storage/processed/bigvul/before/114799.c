__xmlLastError(void) {
    if (IS_MAIN_THREAD)
	return (&xmlLastError);
    else
	return (&xmlGetGlobalState()->xmlLastError);
}

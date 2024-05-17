__xmlLineNumbersDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlLineNumbersDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlLineNumbersDefaultValue);
}

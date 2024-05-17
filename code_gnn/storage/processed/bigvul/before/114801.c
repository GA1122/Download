__xmlLoadExtDtdDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlLoadExtDtdDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlLoadExtDtdDefaultValue);
}

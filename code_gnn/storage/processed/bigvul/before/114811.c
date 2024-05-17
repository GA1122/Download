__xmlRegisterNodeDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlRegisterNodeDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlRegisterNodeDefaultValue);
}

__xmlDeregisterNodeDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlDeregisterNodeDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlDeregisterNodeDefaultValue);
}

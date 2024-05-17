__xmlPedanticParserDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlPedanticParserDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlPedanticParserDefaultValue);
}

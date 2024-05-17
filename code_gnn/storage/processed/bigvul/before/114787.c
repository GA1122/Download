__oldXMLWDcompatibility(void) {
    if (IS_MAIN_THREAD)
	return (&oldXMLWDcompatibility);
    else
	return (&xmlGetGlobalState()->oldXMLWDcompatibility);
}

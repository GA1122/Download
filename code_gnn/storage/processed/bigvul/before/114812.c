__xmlSaveNoEmptyTags(void) {
    if (IS_MAIN_THREAD)
	return (&xmlSaveNoEmptyTags);
    else
	return (&xmlGetGlobalState()->xmlSaveNoEmptyTags);
}

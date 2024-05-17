__xmlSubstituteEntitiesDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlSubstituteEntitiesDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlSubstituteEntitiesDefaultValue);
}

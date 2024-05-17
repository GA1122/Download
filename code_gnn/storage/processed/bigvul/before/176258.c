 explicit FastStringWrapperElementsAccessor(const char* name)
 : StringWrapperElementsAccessor<
 FastStringWrapperElementsAccessor, FastHoleyObjectElementsAccessor,
 ElementsKindTraits<FAST_STRING_WRAPPER_ELEMENTS>>(name) {}

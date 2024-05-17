 explicit SlowStringWrapperElementsAccessor(const char* name)
 : StringWrapperElementsAccessor<
 SlowStringWrapperElementsAccessor, DictionaryElementsAccessor,
 ElementsKindTraits<SLOW_STRING_WRAPPER_ELEMENTS>>(name) {}

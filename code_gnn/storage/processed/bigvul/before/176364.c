 explicit SlowSloppyArgumentsElementsAccessor(const char* name)
 : SloppyArgumentsElementsAccessor<
 SlowSloppyArgumentsElementsAccessor, DictionaryElementsAccessor,
 ElementsKindTraits<SLOW_SLOPPY_ARGUMENTS_ELEMENTS> >(name) {}

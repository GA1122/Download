 explicit FastHoleyDoubleElementsAccessor(const char* name)
 : FastDoubleElementsAccessor<
 FastHoleyDoubleElementsAccessor,
 ElementsKindTraits<FAST_HOLEY_DOUBLE_ELEMENTS> >(name) {}

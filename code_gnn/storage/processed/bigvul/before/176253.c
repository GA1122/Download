 explicit FastPackedDoubleElementsAccessor(const char* name)
 : FastDoubleElementsAccessor<
 FastPackedDoubleElementsAccessor,
 ElementsKindTraits<FAST_DOUBLE_ELEMENTS> >(name) {}

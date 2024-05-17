 explicit FastPackedObjectElementsAccessor(const char* name)
 : FastSmiOrObjectElementsAccessor<
 FastPackedObjectElementsAccessor,
 ElementsKindTraits<FAST_ELEMENTS> >(name) {}

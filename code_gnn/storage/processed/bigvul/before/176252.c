 explicit FastHoleySmiElementsAccessor(const char* name)
 : FastSmiOrObjectElementsAccessor<
 FastHoleySmiElementsAccessor,
 ElementsKindTraits<FAST_HOLEY_SMI_ELEMENTS> >(name) {}

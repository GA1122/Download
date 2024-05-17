 explicit FastHoleyObjectElementsAccessor(const char* name)
 : FastSmiOrObjectElementsAccessor<
 FastHoleyObjectElementsAccessor,
 ElementsKindTraits<FAST_HOLEY_ELEMENTS> >(name) {}

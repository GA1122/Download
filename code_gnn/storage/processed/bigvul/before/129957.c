PRUNTIME_FUNCTION TestUnwindFunctions::LookupFunctionEntry(
    DWORD64 program_counter,
    PDWORD64 image_base) {
  supplied_program_counter_ = program_counter;
  if (custom_image_base_) {
    *image_base = custom_image_base_;
    custom_image_base_ = 0;
  } else {
    *image_base = next_image_base_;
    next_image_base_ += kImageBaseIncrement;
  }
  if (next_lookup_returns_null_) {
    next_lookup_returns_null_ = false;
    return nullptr;
  }

  return kNonNullRuntimeFunctionPointer;
}

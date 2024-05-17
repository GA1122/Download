TestUnwindFunctions::TestUnwindFunctions()
    : supplied_program_counter_(0),
      custom_image_base_(0),
      next_image_base_(kImageBaseIncrement),
      next_lookup_returns_null_(false) {
}

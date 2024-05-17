  static testing::Matcher<const cc::PaintOpBuffer&> Make(
      std::initializer_list<cc::PaintOpType> args) {
    return testing::MakeMatcher(new PaintRecordMatcher(args));
  }

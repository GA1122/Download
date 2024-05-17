  Win32StackFrameUnwinderTest::CreateUnwinder() {
  return make_scoped_ptr(new Win32StackFrameUnwinder(&unwind_functions_));
//   scoped_ptr<TestUnwindFunctions> unwind_functions(new TestUnwindFunctions);
//   unwind_functions_ = unwind_functions.get();
//   return make_scoped_ptr(new Win32StackFrameUnwinder(unwind_functions.Pass()));
  }
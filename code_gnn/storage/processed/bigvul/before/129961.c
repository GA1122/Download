void TestUnwindFunctions::VirtualUnwind(DWORD64 image_base,
                                        DWORD64 program_counter,
                                        PRUNTIME_FUNCTION runtime_function,
                                        CONTEXT* context) {
  EXPECT_EQ(next_image_base_ - kImageBaseIncrement, image_base);
  EXPECT_EQ(supplied_program_counter_, program_counter);
  EXPECT_EQ(kNonNullRuntimeFunctionPointer, runtime_function);
}

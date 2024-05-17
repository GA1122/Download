 void Test_glTerminatePPAPI() {
   GLboolean terminate = glTerminatePPAPI();
  EXPECT(terminate == true);
  TEST_PASSED;
}

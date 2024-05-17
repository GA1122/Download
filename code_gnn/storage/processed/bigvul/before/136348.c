void PaintPropertyTreeBuilderTest::LoadTestData(const char* file_name) {
  String full_path = test::BlinkRootDir();
  full_path.append("/renderer/core/paint/test_data/");
  full_path.append(file_name);
  const Vector<char> input_buffer = test::ReadFromFile(full_path)->Copy();
  SetBodyInnerHTML(String(input_buffer.data(), input_buffer.size()));
}

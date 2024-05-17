std::string TransformToColMajorString(gfx::Transform& t) {
  float array[16];
  t.matrix().asColMajorf(array);
  std::string array_string = "[";
  for (int i = 0; i < 16; i++) {
    array_string += base::NumberToString(array[i]) + ",";
  }
  array_string.pop_back();
  array_string.push_back(']');
  return array_string;
}

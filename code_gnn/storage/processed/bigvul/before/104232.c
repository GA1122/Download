static bool ValueInArray(GLint value, GLint* array, GLint count) {
  for (GLint ii = 0; ii < count; ++ii) {
    if (array[ii] == value) {
      return true;
    }
  }
  return false;
}

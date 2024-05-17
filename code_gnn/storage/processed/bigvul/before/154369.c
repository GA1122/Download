static bool StringIsValidForGLES(const std::string& str) {
  return str.length() == 0 ||
         std::find_if_not(str.begin(), str.end(), CharacterIsValidForGLES) ==
             str.end();
}

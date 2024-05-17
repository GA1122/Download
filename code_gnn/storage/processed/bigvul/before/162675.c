const char* RemoveURLWhitespace(const char* input,
                                int input_len,
                                CanonOutputT<char>* buffer,
                                int* output_len,
                                bool* potentially_dangling_markup) {
  return DoRemoveURLWhitespace(input, input_len, buffer, output_len,
                               potentially_dangling_markup);
}

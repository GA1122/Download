const base::char16* RemoveURLWhitespace(const base::char16* input,
                                        int input_len,
                                        CanonOutputT<base::char16>* buffer,
                                        int* output_len,
                                        bool* potentially_dangling_markup) {
  return DoRemoveURLWhitespace(input, input_len, buffer, output_len,
                               potentially_dangling_markup);
}

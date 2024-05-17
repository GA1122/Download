void BrotliSetCustomDictionary(
    size_t size, const uint8_t* dict, BrotliState* s) {
  s->custom_dict = dict;
  s->custom_dict_size = (int) size;
}

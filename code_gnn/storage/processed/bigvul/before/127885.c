static inline bool VerifyCharacters(const uint8* buffer,
                                    int buffer_size,
                                    int* offset,
                                    char c1,
                                    char c2) {
  RCHECK(*offset < buffer_size);
  char c = static_cast<char>(buffer[(*offset)++]);
  return (c == c1 || (c == c2 && c2 != 0));
}

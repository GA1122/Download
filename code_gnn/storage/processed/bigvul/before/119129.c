bool Truncated(const std::string& input,
               const size_t byte_size,
               std::string* output) {
    size_t prev = input.length();
    TruncateUTF8ToByteSize(input, byte_size, output);
    return prev != output->length();
}

static void writeBoolAsInt(void*& buffer, size_t& size, bool b) {
 FlattenableUtils::write(buffer, size, static_cast<int32_t>(b));
}

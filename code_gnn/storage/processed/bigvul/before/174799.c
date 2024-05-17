static bool readBoolFromInt(void const*& buffer, size_t& size) {
 int32_t i;
 FlattenableUtils::read(buffer, size, i);
 return static_cast<bool>(i);
}

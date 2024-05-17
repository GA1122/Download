status_t Parcel::writeUtf8VectorAsUtf16Vector(
 const std::unique_ptr<std::vector<std::unique_ptr<std::string>>>& val) {
 return writeNullableTypedVector(val, &Parcel::writeUtf8AsUtf16);
}

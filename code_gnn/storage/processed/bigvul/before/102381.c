bool DecodeBQEncoding(const std::string& part,
                      RFC2047EncodingType enc_type,
                      const std::string& charset,
                      std::string* output) {
  std::string decoded;
  if (!((enc_type == B_ENCODING) ?
      base::Base64Decode(part, &decoded) : QPDecode(part, &decoded)))
    return false;

  if (decoded.empty()) {
    output->clear();
    return true;
  }

  UErrorCode err = U_ZERO_ERROR;
  UConverter* converter(ucnv_open(charset.c_str(), &err));
  if (U_FAILURE(err))
    return false;

  size_t output_length = decoded.length() * 3 + 1;
  char* buf = WriteInto(output, output_length);
  output_length = ucnv_toAlgorithmic(UCNV_UTF8, converter, buf, output_length,
                                     decoded.data(), decoded.length(), &err);
  ucnv_close(converter);
  if (U_FAILURE(err))
    return false;
  output->resize(output_length);
  return true;
}

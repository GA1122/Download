static bool IsEntireResource(const ResourceResponse& response) {
  if (response.HttpStatusCode() != 206)
    return true;

  int64_t first_byte_position = -1, last_byte_position = -1,
          instance_length = -1;
  return ParseContentRangeHeaderFor206(
             response.HttpHeaderField("Content-Range"), &first_byte_position,
             &last_byte_position, &instance_length) &&
         first_byte_position == 0 && last_byte_position + 1 == instance_length;
}

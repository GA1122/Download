bool HasGzipHeader(const base::RefCountedMemory& maybe_gzipped) {
  net::GZipHeader header;
  net::GZipHeader::Status header_status = net::GZipHeader::INCOMPLETE_HEADER;
  const char* header_end = nullptr;
  while (header_status == net::GZipHeader::INCOMPLETE_HEADER) {
    header_status = header.ReadMore(maybe_gzipped.front_as<char>(),
                                    maybe_gzipped.size(),
                                    &header_end);
  }
  return header_status == net::GZipHeader::COMPLETE_HEADER;
}

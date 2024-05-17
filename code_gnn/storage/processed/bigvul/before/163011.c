std::unique_ptr<base::Value> ReadDumpFile(const base::FilePath& path) {
  using base::File;
  File dumpfile(path, File::FLAG_OPEN | File::FLAG_READ);

#if defined(OS_WIN)
  int fd = _open_osfhandle(
      reinterpret_cast<intptr_t>(dumpfile.TakePlatformFile()), 0);
#else
  int fd = dumpfile.TakePlatformFile();
#endif
  gzFile gz_file = gzdopen(fd, "r");
  if (!gz_file) {
    LOG(ERROR) << "Cannot open compressed trace file";
    return nullptr;
  }

  std::string dump_string;

  char buf[4096];
  int bytes_read;
  while ((bytes_read = gzread(gz_file, buf, sizeof(buf))) == sizeof(buf)) {
    dump_string.append(buf, bytes_read);
  }
  if (bytes_read < 0) {
    LOG(ERROR) << "Error reading file";
    return nullptr;
  }
  dump_string.append(buf, bytes_read);   
  if (dump_string.size() == 0) {
    return nullptr;
  }

  return base::JSONReader::Read(dump_string);
}

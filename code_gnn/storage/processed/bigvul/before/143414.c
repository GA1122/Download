 scoped_refptr<ProxyResolverScriptData> LoadScriptData(const char* filename) {
  base::FilePath path;
  PathService::Get(base::DIR_SOURCE_ROOT, &path);
  path = path.AppendASCII("net");
  path = path.AppendASCII("data");
  path = path.AppendASCII("proxy_resolver_v8_tracing_unittest");
  path = path.AppendASCII(filename);

  std::string file_contents;
  bool ok = base::ReadFileToString(path, &file_contents);

  EXPECT_TRUE(ok) << "Failed to read file: " << path.value();

  return ProxyResolverScriptData::FromUTF8(file_contents);
}

TestContentClient::TestContentClient() {
  FilePath content_resources_pack_path;
  PathService::Get(base::DIR_MODULE, &content_resources_pack_path);
  content_resources_pack_path = content_resources_pack_path.Append(
      FILE_PATH_LITERAL("content_resources.pak"));
  data_pack_.Load(content_resources_pack_path);
}

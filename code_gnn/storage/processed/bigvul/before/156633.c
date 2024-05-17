std::unique_ptr<content::BlobHandle> CreateMemoryBackedBlob(
    BrowserContext* browser_context,
    const std::string& contents,
    const std::string& content_type) {
  std::unique_ptr<content::BlobHandle> result;
  base::RunLoop loop;
  BrowserContext::CreateMemoryBackedBlob(
      browser_context, contents.c_str(), contents.length(), content_type,
      base::BindOnce(
          [](std::unique_ptr<content::BlobHandle>* out_blob,
             base::OnceClosure done,
             std::unique_ptr<content::BlobHandle> blob) {
            *out_blob = std::move(blob);
            std::move(done).Run();
          },
          &result, loop.QuitClosure()));
  loop.Run();
  EXPECT_TRUE(result);
  return result;
}

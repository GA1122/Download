const Extension* ExtensionBrowserTest::GetExtensionByPath(
    const extensions::ExtensionSet& extensions,
    const base::FilePath& path) {
  base::ThreadRestrictions::ScopedAllowIO allow_io;
  base::FilePath extension_path = base::MakeAbsoluteFilePath(path);
  EXPECT_TRUE(!extension_path.empty());
  for (const scoped_refptr<const Extension>& extension : extensions) {
    if (extension->path() == extension_path) {
      return extension.get();
    }
  }
  return NULL;
}

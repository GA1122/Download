  scoped_refptr<const extensions::Extension> LoadUnpackedExtension(
      const base::FilePath::StringType& name) {
    base::FilePath extension_path(ui_test_utils::GetTestFilePath(
        base::FilePath(kTestExtensionsDir), base::FilePath(name)));
    extensions::ChromeTestExtensionLoader loader(browser()->profile());
    return loader.LoadExtension(extension_path);
  }

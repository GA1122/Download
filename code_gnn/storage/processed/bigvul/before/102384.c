void EnsureSafeExtension(const std::string& mime_type,
                         bool ignore_extension,
                         FilePath* file_name) {
  FilePath::StringType extension = file_name->Extension();
  if (!extension.empty())
    extension.erase(extension.begin());   

  if ((ignore_extension || extension.empty()) && !mime_type.empty()) {
    FilePath::StringType mime_extension;
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    net::GetPreferredExtensionForMimeType(mime_type, &mime_extension);
    if (!mime_extension.empty())
      extension = mime_extension;
  }

#if defined(OS_WIN)
  static const FilePath::CharType default_extension[] =
      FILE_PATH_LITERAL("download");

  if (IsShellIntegratedExtension(extension))
    extension.assign(default_extension);
#endif

  *file_name = file_name->ReplaceExtension(extension);
}

void GetExecutableVersionDetails(base::string16* product_name,
                                 base::string16* version_number,
                                 base::string16* channel_name) {
  DCHECK_NE(nullptr, product_name);
  DCHECK_NE(nullptr, version_number);
  DCHECK_NE(nullptr, channel_name);

  wchar_t exe_file[MAX_PATH] = {};
  CHECK(::GetModuleFileName(nullptr, exe_file, arraysize(exe_file)));

  base::string16 unused_special_build;
  install_static::GetExecutableVersionDetails(
      exe_file, product_name, version_number, &unused_special_build,
      channel_name);
}

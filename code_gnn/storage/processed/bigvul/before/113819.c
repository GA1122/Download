bool AddDirectory(int path, const wchar_t* sub_dir, bool children,
                  sandbox::TargetPolicy::Semantics access,
                  sandbox::TargetPolicy* policy) {
  FilePath directory;
  if (!PathService::Get(path, &directory))
    return false;

  if (sub_dir) {
    directory = directory.Append(sub_dir);
    file_util::AbsolutePath(&directory);
  }

  sandbox::ResultCode result;
  result = policy->AddRule(sandbox::TargetPolicy::SUBSYS_FILES, access,
                           directory.value().c_str());
  if (result != sandbox::SBOX_ALL_OK)
    return false;

  std::wstring directory_str = directory.value() + L"\\";
  if (children)
    directory_str += L"*";

  result = policy->AddRule(sandbox::TargetPolicy::SUBSYS_FILES, access,
                           directory_str.c_str());
  if (result != sandbox::SBOX_ALL_OK)
    return false;

  return true;
}

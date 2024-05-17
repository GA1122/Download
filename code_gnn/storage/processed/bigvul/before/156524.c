bool ChildProcessSecurityPolicyImpl::CanReadAllFiles(
    int child_id,
    const std::vector<base::FilePath>& files) {
  return std::all_of(files.begin(), files.end(),
                     [this, child_id](const base::FilePath& file) {
                       return CanReadFile(child_id, file);
                     });
}

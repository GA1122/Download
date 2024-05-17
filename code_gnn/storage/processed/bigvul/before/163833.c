net::URLRequestJob* ExtensionProtocolTestHandler(
    const base::FilePath& test_dir_root,
    net::URLRequest* request,
    net::NetworkDelegate* network_delegate,
    const base::FilePath& relative_path) {
  if (!base::FilePath(FILE_PATH_LITERAL("_test_resources"))
           .IsParent(relative_path)) {
    return nullptr;
  }

  std::vector<base::FilePath::StringType> components;
  relative_path.GetComponents(&components);
  DCHECK_GT(components.size(), 1u);
  base::FilePath resource_path = test_dir_root;
  for (size_t i = 1u; i < components.size(); ++i)
    resource_path = resource_path.Append(components[i]);

  return new net::URLRequestFileJob(
      request, network_delegate, resource_path,
      base::CreateTaskRunnerWithTraits(
          {base::MayBlock(), base::TaskPriority::BACKGROUND,
           base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}));
}

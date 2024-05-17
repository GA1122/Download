bool BuildEnvironmentWithApk(std::vector<std::string>* result) {
  DCHECK(result->empty());

  base::FilePath apk_path;
  if (!base::android::GetPathToBaseApk(&apk_path)) {
    return false;
  }

  std::unique_ptr<base::Environment> env(base::Environment::Create());
  static constexpr char kClasspathVar[] = "CLASSPATH";
  std::string classpath;
  env->GetVar(kClasspathVar, &classpath);
  classpath = apk_path.value() + ":" + classpath;

  static constexpr char kLdLibraryPathVar[] = "LD_LIBRARY_PATH";
  std::string library_path;
  env->GetVar(kLdLibraryPathVar, &library_path);
  library_path = apk_path.value() + "!/lib/" CURRENT_ABI ":" + library_path;

  result->push_back("CLASSPATH=" + classpath);
  result->push_back("LD_LIBRARY_PATH=" + library_path);
  for (char** envp = environ; *envp != nullptr; ++envp) {
    if ((strncmp(*envp, kClasspathVar, strlen(kClasspathVar)) == 0 &&
         (*envp)[strlen(kClasspathVar)] == '=') ||
        (strncmp(*envp, kLdLibraryPathVar, strlen(kLdLibraryPathVar)) == 0 &&
         (*envp)[strlen(kLdLibraryPathVar)] == '=')) {
      continue;
    }
    result->push_back(*envp);
  }

  return true;
}

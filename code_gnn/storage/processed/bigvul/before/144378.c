base::string16 ExtensionInstallPrompt::Prompt::GetRetainedFile(size_t index)
    const {
  CHECK_LT(index, retained_files_.size());
  return retained_files_[index].AsUTF16Unsafe();
}

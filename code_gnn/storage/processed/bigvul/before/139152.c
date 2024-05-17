base::FilePath RenderProcessHostImpl::GetAecDumpFilePathWithExtensions(
    const base::FilePath& file) {
  return file.AddExtension(IntToStringType(base::GetProcId(GetHandle())))
      .AddExtension(kAecDumpFileNameAddition);
}

ChildProcessLauncherHelper::GetFilesToMap() {
  DCHECK(CurrentlyOnProcessLauncherTaskRunner());

  CHECK(!command_line()->HasSwitch(switches::kSingleProcess));

  std::unique_ptr<PosixFileDescriptorInfo> files_to_register =
      CreateDefaultPosixFilesToMap(child_process_id(),
                                   mojo_channel_->remote_endpoint(),
                                   true  ,
                                   GetProcessType(), command_line());

#if ICU_UTIL_DATA_IMPL == ICU_UTIL_DATA_FILE
  base::MemoryMappedFile::Region icu_region;
  int fd = base::i18n::GetIcuDataFileHandle(&icu_region);
  files_to_register->ShareWithRegion(kAndroidICUDataDescriptor, fd, icu_region);
#endif   

  return files_to_register;
}

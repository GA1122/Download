bool CanGetFilesId() {
  return SystemSaltGetter::Get()->GetRawSalt();
}

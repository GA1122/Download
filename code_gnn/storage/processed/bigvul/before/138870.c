void CallWhenCanGetFilesId(const base::Closure& closure) {
  SystemSaltGetter::Get()->AddOnSystemSaltReady(closure);
}

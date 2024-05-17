static bool is_shell_running(void) {
 char property_str[100];
  property_get("init.svc.zygote", property_str, NULL);
 if (!strcmp("running", property_str)) {
 return true;
 }
 return false;
}

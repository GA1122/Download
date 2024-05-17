static bool is_wifi_interface(const char *name)
{
 if (strncmp(name, "wlan", 4) != 0 && strncmp(name, "p2p", 3) != 0) {
  
 return false;
 } else {
 return true;
 }
}

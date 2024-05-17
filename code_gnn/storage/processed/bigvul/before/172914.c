bool adapter_repeated_enable_disable() {
 for (int i = 0; i < 10; ++i) {
 if (!adapter_enable_disable()) {
 return false;
 }
 }
 return true;
}

bool InputDispatcher::isAppSwitchKeyCode(int32_t keyCode) {
 return keyCode == AKEYCODE_HOME
 || keyCode == AKEYCODE_ENDCALL
 || keyCode == AKEYCODE_APP_SWITCH;
}

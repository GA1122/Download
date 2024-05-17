static bool validateKeyEvent(int32_t action) {
 if (! isValidKeyAction(action)) {
        ALOGE("Key event has invalid action code 0x%x", action);
 return false;
 }
 return true;
}

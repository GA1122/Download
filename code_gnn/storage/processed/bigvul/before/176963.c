static bool isValidKeyAction(int32_t action) {
 switch (action) {
 case AKEY_EVENT_ACTION_DOWN:
 case AKEY_EVENT_ACTION_UP:
 return true;
 default:
 return false;
 }
}

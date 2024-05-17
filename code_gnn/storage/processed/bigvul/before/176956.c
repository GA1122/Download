bool InputDispatcher::isAppSwitchKeyEventLocked(KeyEntry* keyEntry) {
 return ! (keyEntry->flags & AKEY_EVENT_FLAG_CANCELED)
 && isAppSwitchKeyCode(keyEntry->keyCode)
 && (keyEntry->policyFlags & POLICY_FLAG_TRUSTED)
 && (keyEntry->policyFlags & POLICY_FLAG_PASS_TO_USER);
}

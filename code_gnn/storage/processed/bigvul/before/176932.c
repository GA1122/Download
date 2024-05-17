void InputDispatcher::TouchState::filterNonAsIsTouchWindows() {
 for (size_t i = 0 ; i < windows.size(); ) {
 TouchedWindow& window = windows.editItemAt(i);
 if (window.targetFlags & (InputTarget::FLAG_DISPATCH_AS_IS
 | InputTarget::FLAG_DISPATCH_AS_SLIPPERY_ENTER)) {
            window.targetFlags &= ~InputTarget::FLAG_DISPATCH_MASK;
            window.targetFlags |= InputTarget::FLAG_DISPATCH_AS_IS;
            i += 1;
 } else {
            windows.removeAt(i);
 }
 }
}

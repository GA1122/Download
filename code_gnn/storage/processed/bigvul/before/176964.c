static bool isValidMotionAction(int32_t action, int32_t actionButton, int32_t pointerCount) {
 switch (action & AMOTION_EVENT_ACTION_MASK) {
 case AMOTION_EVENT_ACTION_DOWN:
 case AMOTION_EVENT_ACTION_UP:
 case AMOTION_EVENT_ACTION_CANCEL:
 case AMOTION_EVENT_ACTION_MOVE:
 case AMOTION_EVENT_ACTION_OUTSIDE:
 case AMOTION_EVENT_ACTION_HOVER_ENTER:
 case AMOTION_EVENT_ACTION_HOVER_MOVE:
 case AMOTION_EVENT_ACTION_HOVER_EXIT:
 case AMOTION_EVENT_ACTION_SCROLL:
 return true;
 case AMOTION_EVENT_ACTION_POINTER_DOWN:
 case AMOTION_EVENT_ACTION_POINTER_UP: {
 int32_t index = getMotionEventActionPointerIndex(action);
 return index >= 0 && size_t(index) < pointerCount;
 }
 case AMOTION_EVENT_ACTION_BUTTON_PRESS:
 case AMOTION_EVENT_ACTION_BUTTON_RELEASE:
 return actionButton != 0;
 default:
 return false;
 }
}

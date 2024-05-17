static bool WebAccessibilityNotificationToViewHostMsg(
    WebAccessibilityNotification notification,
    ViewHostMsg_AccessibilityNotification_Type::Value* type) {
  switch (notification) {
    case WebKit::WebAccessibilityNotificationCheckedStateChanged:
      *type = ViewHostMsg_AccessibilityNotification_Type::
          NOTIFICATION_TYPE_CHECK_STATE_CHANGED;
      break;
    case WebKit::WebAccessibilityNotificationChildrenChanged:
      *type = ViewHostMsg_AccessibilityNotification_Type::
          NOTIFICATION_TYPE_CHILDREN_CHANGED;
      break;
    case WebKit::WebAccessibilityNotificationFocusedUIElementChanged:
      *type = ViewHostMsg_AccessibilityNotification_Type::
           NOTIFICATION_TYPE_FOCUS_CHANGED;
      break;
    case WebKit::WebAccessibilityNotificationLoadComplete:
      *type = ViewHostMsg_AccessibilityNotification_Type::
          NOTIFICATION_TYPE_LOAD_COMPLETE;
      break;
    case WebKit::WebAccessibilityNotificationValueChanged:
      *type = ViewHostMsg_AccessibilityNotification_Type::
          NOTIFICATION_TYPE_VALUE_CHANGED;
      break;
    case WebKit::WebAccessibilityNotificationSelectedTextChanged:
      *type = ViewHostMsg_AccessibilityNotification_Type::
          NOTIFICATION_TYPE_SELECTED_TEXT_CHANGED;
      break;
    default:
      return false;
  }
  return true;
}

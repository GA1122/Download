bool adapter_start_discovery() {
 int error;

  CALL_AND_WAIT(error = bt_interface->start_discovery(), discovery_state_changed);
  TASSERT(error == BT_STATUS_SUCCESS, "Error calling start_discovery: %d", error);
  TASSERT(adapter_get_discovery_state() == BT_DISCOVERY_STARTED, "Unable to start discovery.");

 return true;
}

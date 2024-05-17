bool adapter_get_name() {
 int error;
 bt_property_t *name = property_new_name("get_name");

  CALL_AND_WAIT(bt_interface->set_adapter_property(name), adapter_properties);
  CALL_AND_WAIT(error = bt_interface->get_adapter_property(BT_PROPERTY_BDNAME), adapter_properties);
  TASSERT(error == BT_STATUS_SUCCESS, "Error getting device name.");
  TASSERT(adapter_get_property_count() == 1, "Expected 1 adapter property change, found %d instead.", adapter_get_property_count());
  TASSERT(adapter_get_property(BT_PROPERTY_BDNAME), "The Bluetooth name property did not change.");
  TASSERT(property_equals(adapter_get_property(BT_PROPERTY_BDNAME), name), "Bluetooth name '%s' does not match test value", property_as_name(adapter_get_property(BT_PROPERTY_BDNAME))->name);

  property_free(name);
 return true;
}

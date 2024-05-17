const hci_t *hci_layer_get_test_interface(
 const allocator_t *buffer_allocator_interface,
 const hci_hal_t *hal_interface,
 const btsnoop_t *btsnoop_interface,
 const hci_inject_t *hci_inject_interface,
 const packet_fragmenter_t *packet_fragmenter_interface,
 const vendor_t *vendor_interface,
 const low_power_manager_t *low_power_manager_interface) {

  buffer_allocator = buffer_allocator_interface;
  hal = hal_interface;
  btsnoop = btsnoop_interface;
  hci_inject = hci_inject_interface;
  packet_fragmenter = packet_fragmenter_interface;
  vendor = vendor_interface;
  low_power_manager = low_power_manager_interface;

  init_layer_interface();
 return &interface;
}

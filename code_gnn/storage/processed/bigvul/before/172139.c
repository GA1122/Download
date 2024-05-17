static void init_layer_interface() {
 if (!interface_created) {
    interface.send_low_power_command = low_power_manager->post_command;
    interface.do_postload = do_postload;

    interface.event_dispatcher = data_dispatcher_new("hci_layer");
 if (!interface.event_dispatcher) {
      LOG_ERROR("%s could not create upward dispatcher.", __func__);
 return;
 }

    interface.set_data_queue = set_data_queue;
    interface.transmit_command = transmit_command;
    interface.transmit_command_futured = transmit_command_futured;
    interface.transmit_downward = transmit_downward;
    interface_created = true;
 }
}

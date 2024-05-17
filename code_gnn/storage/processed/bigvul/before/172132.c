static serial_data_type_t event_to_data_type(uint16_t event) {
 if (event == MSG_STACK_TO_HC_HCI_ACL)
 return DATA_TYPE_ACL;
 else if (event == MSG_STACK_TO_HC_HCI_SCO)
 return DATA_TYPE_SCO;
 else if (event == MSG_STACK_TO_HC_HCI_CMD)
 return DATA_TYPE_COMMAND;
 else
    LOG_ERROR("%s invalid event type, could not translate 0x%x", __func__, event);

 return 0;
}

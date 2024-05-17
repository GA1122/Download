void red_channel_client_set_header_sub_list(RedChannelClient *rcc, uint32_t sub_list)
{
    rcc->send_data.header.set_msg_sub_list(&rcc->send_data.header, sub_list);
}

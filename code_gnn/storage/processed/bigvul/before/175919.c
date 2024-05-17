void smp_key_pick_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t key_to_dist =
 (p_cb->role == HCI_ROLE_SLAVE) ? p_cb->local_r_key : p_cb->local_i_key;
 uint8_t i = 0;

  SMP_TRACE_DEBUG("%s key_to_dist=0x%x", __func__, key_to_dist);
 while (i < SMP_KEY_DIST_TYPE_MAX) {
    SMP_TRACE_DEBUG("key to send = %02x, i = %d", key_to_dist, i);

 if (key_to_dist & (1 << i)) {
      SMP_TRACE_DEBUG("smp_distribute_act[%d]", i);
 (*smp_distribute_act[i])(p_cb, p_data);
 break;
 }
    i++;
 }
}

static void jv_dm_cback(tBTA_JV_EVT event, tBTA_JV *p_data, void *user_data) {
 uint32_t id = (uintptr_t)user_data;
 switch(event) {
 case BTA_JV_GET_SCN_EVT:
 {
      pthread_mutex_lock(&slot_lock);
 rfc_slot_t* rs = find_rfc_slot_by_id(id);
 int new_scn = p_data->scn;

 if(rs && (new_scn != 0))
 {
        rs->scn = new_scn;
  

 if(!send_app_scn(rs)){
          APPL_TRACE_DEBUG("send_app_scn() failed, close rs->id:%d", rs->id);
          cleanup_rfc_slot(rs);
 } else {
 if(rs->is_service_uuid_valid == true) {
            BTA_JvCreateRecordByUser((void *)rs->id);
 } else {
            APPL_TRACE_DEBUG("is_service_uuid_valid==false - don't set SDP-record, "
 "just start the RFCOMM server", rs->id);
            BTA_JvRfcommStartServer(rs->security, rs->role, rs->scn, MAX_RFC_SESSION,
                    rfcomm_cback, (void*)rs->id);
 }
 }
 } else if(rs) {
        APPL_TRACE_ERROR("jv_dm_cback: Error: allocate channel %d, slot found:%p", rs->scn, rs);
        cleanup_rfc_slot(rs);
 }
      pthread_mutex_unlock(&slot_lock);
 break;
 }
 case BTA_JV_GET_PSM_EVT:
 {
      APPL_TRACE_DEBUG("Received PSM: 0x%04x", p_data->psm);
      on_l2cap_psm_assigned(id, p_data->psm);
 break;
 }
 case BTA_JV_CREATE_RECORD_EVT: {
      pthread_mutex_lock(&slot_lock);

 rfc_slot_t *slot = find_rfc_slot_by_id(id);
 if (slot && create_server_sdp_record(slot)) {
        BTA_JvRfcommStartServer(slot->security, slot->role, slot->scn, MAX_RFC_SESSION, rfcomm_cback, (void *)(uintptr_t)slot->id);
 } else if(slot) {
        APPL_TRACE_ERROR("jv_dm_cback: cannot start server, slot found:%p", slot);
        cleanup_rfc_slot(slot);
 }

      pthread_mutex_unlock(&slot_lock);
 break;
 }

 case BTA_JV_DISCOVERY_COMP_EVT: {
      pthread_mutex_lock(&slot_lock);
 rfc_slot_t *slot = find_rfc_slot_by_id(id);
 if (p_data->disc_comp.status == BTA_JV_SUCCESS && p_data->disc_comp.scn) {
 if (slot && slot->f.doing_sdp_request) {
 if (BTA_JvRfcommConnect(slot->security, slot->role, p_data->disc_comp.scn, slot->addr.address, rfcomm_cback, (void *)(uintptr_t)slot->id) == BTA_JV_SUCCESS) {
            slot->scn = p_data->disc_comp.scn;
            slot->f.doing_sdp_request = false;
 if (!send_app_scn(slot))
              cleanup_rfc_slot(slot);
 } else {
            cleanup_rfc_slot(slot);
 }
 } else if (slot) {
          LOG_ERROR("%s SDP response returned but RFCOMM slot %d did not request SDP record.", __func__, id);
 }
 } else if (slot) {
        cleanup_rfc_slot(slot);
 }

      slot = find_rfc_slot_by_pending_sdp();
 if (slot) {
        tSDP_UUID sdp_uuid;
        sdp_uuid.len = 16;
        memcpy(sdp_uuid.uu.uuid128, slot->service_uuid, sizeof(sdp_uuid.uu.uuid128));
        BTA_JvStartDiscovery((uint8_t *)slot->addr.address, 1, &sdp_uuid, (void *)(uintptr_t)slot->id);
        slot->f.pending_sdp_request = false;
        slot->f.doing_sdp_request = true;
 }

      pthread_mutex_unlock(&slot_lock);
 break;
 }

 default:
      APPL_TRACE_DEBUG("unhandled event:%d, slot id:%d", event, id);
 break;
 }
}

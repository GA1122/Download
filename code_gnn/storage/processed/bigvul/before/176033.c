void bta_av_rc_msg(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  tBTA_AV_EVT evt = 0;
  tBTA_AV av;
  BT_HDR* p_pkt = NULL;
  tAVRC_MSG_VENDOR* p_vendor = &p_data->rc_msg.msg.vendor;
 bool is_inquiry = ((p_data->rc_msg.msg.hdr.ctype == AVRC_CMD_SPEC_INQ) ||
                     p_data->rc_msg.msg.hdr.ctype == AVRC_CMD_GEN_INQ);
 uint8_t ctype = 0;
  tAVRC_RESPONSE rc_rsp;

  rc_rsp.rsp.status = BTA_AV_STS_NO_RSP;

 if (NULL == p_data) {
    APPL_TRACE_ERROR("%s: Message from peer with no data", __func__);
 return;
 }

  APPL_TRACE_DEBUG("%s: opcode=%x, ctype=%x", __func__, p_data->rc_msg.opcode,
                   p_data->rc_msg.msg.hdr.ctype);

 if (p_data->rc_msg.opcode == AVRC_OP_PASS_THRU) {
  
 if ((p_data->rc_msg.msg.hdr.ctype == AVRC_CMD_CTRL) ||
 (p_data->rc_msg.msg.hdr.ctype == AVRC_CMD_SPEC_INQ) ||
 (p_data->rc_msg.msg.hdr.ctype == AVRC_CMD_GEN_INQ)) {
  
 char avrcp_ct_support[PROPERTY_VALUE_MAX];
      osi_property_get("bluetooth.pts.avrcp_ct.support", avrcp_ct_support,
 "false");
 if (p_data->rc_msg.msg.pass.op_id == AVRC_ID_VENDOR) {
        p_data->rc_msg.msg.hdr.ctype = AVRC_RSP_NOT_IMPL;
 if (p_cb->features & BTA_AV_FEAT_METADATA)
          p_data->rc_msg.msg.hdr.ctype = bta_av_group_navi_supported(
              p_data->rc_msg.msg.pass.pass_len,
              p_data->rc_msg.msg.pass.p_pass_data, is_inquiry);
 } else if (((p_data->rc_msg.msg.pass.op_id == AVRC_ID_VOL_UP) ||
 (p_data->rc_msg.msg.pass.op_id == AVRC_ID_VOL_DOWN)) &&
 !strcmp(avrcp_ct_support, "true")) {
        p_data->rc_msg.msg.hdr.ctype = AVRC_RSP_ACCEPT;
 } else {
        p_data->rc_msg.msg.hdr.ctype =
            bta_av_op_supported(p_data->rc_msg.msg.pass.op_id, is_inquiry);
 }

      APPL_TRACE_DEBUG("%s: ctype %d", __func__, p_data->rc_msg.msg.hdr.ctype)

  
 if (p_data->rc_msg.msg.hdr.ctype != AVRC_RSP_INTERIM)
        AVRC_PassRsp(p_data->rc_msg.handle, p_data->rc_msg.label,
 &p_data->rc_msg.msg.pass);

  
 if (p_data->rc_msg.msg.hdr.ctype == AVRC_RSP_ACCEPT ||
          p_data->rc_msg.msg.hdr.ctype == AVRC_RSP_INTERIM) {
        evt = BTA_AV_REMOTE_CMD_EVT;
        av.remote_cmd.rc_id = p_data->rc_msg.msg.pass.op_id;
        av.remote_cmd.key_state = p_data->rc_msg.msg.pass.state;
        av.remote_cmd.p_data = p_data->rc_msg.msg.pass.p_pass_data;
        av.remote_cmd.len = p_data->rc_msg.msg.pass.pass_len;
        memcpy(&av.remote_cmd.hdr, &p_data->rc_msg.msg.hdr, sizeof(tAVRC_HDR));
        av.remote_cmd.label = p_data->rc_msg.label;
 }
 }
  
  
 else if (p_data->rc_msg.msg.hdr.ctype >= AVRC_RSP_NOT_IMPL) {
  
      evt = BTA_AV_REMOTE_RSP_EVT;
      av.remote_rsp.rc_id = p_data->rc_msg.msg.pass.op_id;
      av.remote_rsp.key_state = p_data->rc_msg.msg.pass.state;
      av.remote_rsp.rsp_code = p_data->rc_msg.msg.hdr.ctype;
      av.remote_rsp.label = p_data->rc_msg.label;

  
 if ((p_data->rc_msg.msg.pass.op_id == AVRC_ID_VENDOR) &&
 (p_data->rc_msg.msg.pass.pass_len > 0)) {
        av.remote_rsp.p_data =
 (uint8_t*)osi_malloc(p_data->rc_msg.msg.pass.pass_len);
        APPL_TRACE_DEBUG("%s: Vendor Unique data len = %d", __func__,
                         p_data->rc_msg.msg.pass.pass_len);
        memcpy(av.remote_rsp.p_data, p_data->rc_msg.msg.pass.p_pass_data,
               p_data->rc_msg.msg.pass.pass_len);
 }
 }
  
 else {
      p_data->rc_msg.msg.hdr.ctype = AVRC_RSP_REJ;
      AVRC_PassRsp(p_data->rc_msg.handle, p_data->rc_msg.label,
 &p_data->rc_msg.msg.pass);
 }
 }
  
 else if (p_data->rc_msg.opcode == AVRC_OP_VENDOR) {
  
    av.vendor_cmd.code = p_data->rc_msg.msg.hdr.ctype;
    av.vendor_cmd.company_id = p_vendor->company_id;
    av.vendor_cmd.label = p_data->rc_msg.label;
    av.vendor_cmd.p_data = p_vendor->p_vendor_data;
    av.vendor_cmd.len = p_vendor->vendor_len;

  
 if ((p_cb->features & BTA_AV_FEAT_VENDOR) &&
        p_data->rc_msg.msg.hdr.ctype <= AVRC_CMD_GEN_INQ) {
 if ((p_cb->features & BTA_AV_FEAT_METADATA) &&
 (p_vendor->company_id == AVRC_CO_METADATA)) {
        av.meta_msg.p_msg = &p_data->rc_msg.msg;
        rc_rsp.rsp.status = BTA_AV_STS_NO_RSP;
        evt = bta_av_proc_meta_cmd(&rc_rsp, &p_data->rc_msg, &ctype);
 } else {
        evt = BTA_AV_VENDOR_CMD_EVT;
 }
 } else if ((p_cb->features & BTA_AV_FEAT_VENDOR) &&
               p_data->rc_msg.msg.hdr.ctype >= AVRC_RSP_NOT_IMPL) {
  
 if ((p_cb->features & BTA_AV_FEAT_METADATA) &&
 (p_vendor->company_id == AVRC_CO_METADATA)) {
        av.meta_msg.p_msg = &p_data->rc_msg.msg;
        evt = BTA_AV_META_MSG_EVT;
 } else {
        evt = BTA_AV_VENDOR_RSP_EVT;
 }
 } else if (!(p_cb->features & BTA_AV_FEAT_VENDOR) &&
               p_data->rc_msg.msg.hdr.ctype <= AVRC_CMD_GEN_INQ) {
  
 if (p_data->rc_msg.msg.vendor.p_vendor_data[0] == AVRC_PDU_INVALID) {
  
        p_data->rc_msg.msg.hdr.ctype = AVRC_RSP_REJ;
        p_data->rc_msg.msg.vendor.p_vendor_data[4] = AVRC_STS_BAD_CMD;
 } else {
        p_data->rc_msg.msg.hdr.ctype = AVRC_RSP_NOT_IMPL;
 }
      AVRC_VendorRsp(p_data->rc_msg.handle, p_data->rc_msg.label,
 &p_data->rc_msg.msg.vendor);
 }
 } else if (p_data->rc_msg.opcode == AVRC_OP_BROWSE) {
  
    av.meta_msg.rc_handle = p_data->rc_msg.handle;
    av.meta_msg.company_id = p_vendor->company_id;
    av.meta_msg.code = p_data->rc_msg.msg.hdr.ctype;
    av.meta_msg.label = p_data->rc_msg.label;
    av.meta_msg.p_msg = &p_data->rc_msg.msg;
    av.meta_msg.p_data = p_data->rc_msg.msg.browse.p_browse_data;
    av.meta_msg.len = p_data->rc_msg.msg.browse.browse_len;
    evt = BTA_AV_META_MSG_EVT;
 }

 if (evt == 0 && rc_rsp.rsp.status != BTA_AV_STS_NO_RSP) {
 if (!p_pkt) {
      rc_rsp.rsp.opcode = p_data->rc_msg.opcode;
      AVRC_BldResponse(0, &rc_rsp, &p_pkt);
 }
 if (p_pkt)
      AVRC_MsgReq(p_data->rc_msg.handle, p_data->rc_msg.label, ctype, p_pkt);
 }

  
 if (evt != 0) {
    av.remote_cmd.rc_handle = p_data->rc_msg.handle;
 (*p_cb->p_cback)(evt, &av);
  
    bta_av_rc_free_browse_msg(p_cb, p_data);
 }
}

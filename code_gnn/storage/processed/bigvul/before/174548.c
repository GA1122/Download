void BTM_IoCapRsp(BD_ADDR bd_addr, tBTM_IO_CAP io_cap, tBTM_OOB_DATA oob, tBTM_AUTH_REQ auth_req)
{
    BTM_TRACE_EVENT ("BTM_IoCapRsp: state: %s  oob: %d io_cap: %d",
                      btm_pair_state_descr(btm_cb.pairing_state), oob, io_cap);

 if ( (btm_cb.pairing_state != BTM_PAIR_STATE_WAIT_LOCAL_IOCAPS)
 || (memcmp (btm_cb.pairing_bda, bd_addr, BD_ADDR_LEN) != 0) )
 return;

 if (oob < BTM_OOB_UNKNOWN && io_cap < BTM_IO_CAP_MAX)
 {
        btm_cb.devcb.loc_auth_req   = auth_req;
        btm_cb.devcb.loc_io_caps    = io_cap;

 if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
            auth_req = (BTM_AUTH_DD_BOND | (auth_req&BTM_AUTH_YN_BIT));

        btsnd_hcic_io_cap_req_reply (bd_addr, io_cap, oob, auth_req);
 }
}

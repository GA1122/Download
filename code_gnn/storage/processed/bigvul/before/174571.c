void BTM_SetPinType (UINT8 pin_type, PIN_CODE pin_code, UINT8 pin_code_len)
{
    BTM_TRACE_API ("BTM_SetPinType: pin type %d [variable-0, fixed-1], code %s, length %d",
                    pin_type, (char *) pin_code, pin_code_len);

  
 if ( (btm_cb.cfg.pin_type != pin_type)
 && (btm_cb.devcb.state > BTM_DEV_STATE_WAIT_AFTER_RESET) )
 {
        btsnd_hcic_write_pin_type (pin_type);
 }

    btm_cb.cfg.pin_type     = pin_type;
    btm_cb.cfg.pin_code_len = pin_code_len;
    memcpy (btm_cb.cfg.pin_code, pin_code, pin_code_len);
}

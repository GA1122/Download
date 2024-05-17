e1000e_process_snap_option(E1000ECore *core, uint32_t cmd_and_length)
{
    if (cmd_and_length & E1000_TXD_CMD_SNAP) {
        trace_e1000e_wrn_no_snap_support();
    }
}

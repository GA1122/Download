e1000e_process_ts_option(E1000ECore *core, struct e1000_tx_desc *dp)
{
    if (le32_to_cpu(dp->upper.data) & E1000_TXD_EXTCMD_TSTAMP) {
        trace_e1000e_wrn_no_ts_support();
    }
}

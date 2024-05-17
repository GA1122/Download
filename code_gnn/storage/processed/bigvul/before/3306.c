static void fdctrl_external_reset_sysbus(DeviceState *d)
{
    FDCtrlSysBus *sys = SYSBUS_FDC(d);
    FDCtrl *s = &sys->state;

    fdctrl_reset(s, 0);
}

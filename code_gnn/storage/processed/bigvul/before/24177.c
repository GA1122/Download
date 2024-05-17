ar6000_disconnect(struct ar6_softc *ar)
{
    if ((ar->arConnected == true) || (ar->arConnectPending == true)) {
        wmi_disconnect_cmd(ar->arWmi);
         
        ar->arConnectPending = false;
    }

    return 0;
}

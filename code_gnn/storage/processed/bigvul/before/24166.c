static int ar6000_connectservice(struct ar6_softc               *ar,
                                      struct htc_service_connect_req  *pConnect,
                                      char *pDesc)
{
    int                 status;
    struct htc_service_connect_resp response;

    do {

        A_MEMZERO(&response,sizeof(response));

        status = HTCConnectService(ar->arHtcTarget,
                                   pConnect,
                                   &response);

        if (status) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,(" Failed to connect to %s service status:%d \n",
                              pDesc, status));
            break;
        }
        switch (pConnect->ServiceID) {
            case WMI_CONTROL_SVC :
                if (ar->arWmiEnabled) {
                         
                    wmi_set_control_ep(ar->arWmi, response.Endpoint);
                }
                     
                ar->arControlEp = response.Endpoint;
                break;
            case WMI_DATA_BE_SVC :
                arSetAc2EndpointIDMap(ar, WMM_AC_BE, response.Endpoint);
                break;
            case WMI_DATA_BK_SVC :
                arSetAc2EndpointIDMap(ar, WMM_AC_BK, response.Endpoint);
                break;
            case WMI_DATA_VI_SVC :
                arSetAc2EndpointIDMap(ar, WMM_AC_VI, response.Endpoint);
                 break;
           case WMI_DATA_VO_SVC :
                arSetAc2EndpointIDMap(ar, WMM_AC_VO, response.Endpoint);
                break;
           default:
                AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("ServiceID not mapped %d\n", pConnect->ServiceID));
                status = A_EINVAL;
            break;
        }

    } while (false);

    return status;
}
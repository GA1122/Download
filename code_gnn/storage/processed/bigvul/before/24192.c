void ar6000_indicate_tx_activity(void *devt, u8 TrafficClass, bool Active)
{
    struct ar6_softc  *ar = (struct ar6_softc *)devt;
    HTC_ENDPOINT_ID eid ;
    int i;

    if (ar->arWmiEnabled) {
        eid = arAc2EndpointID(ar, TrafficClass);

        AR6000_SPIN_LOCK(&ar->arLock, 0);

        ar->arAcStreamActive[TrafficClass] = Active;

        if (Active) {
             

            if (ar->arAcStreamPriMap[TrafficClass] > ar->arHiAcStreamActivePri) {
                     
                ar->arHiAcStreamActivePri = ar->arAcStreamPriMap[TrafficClass];
            }

        } else {
             

            if (ar->arHiAcStreamActivePri == ar->arAcStreamPriMap[TrafficClass]) {

                 

                     
                ar->arHiAcStreamActivePri = 0;
                for (i = 0; i < WMM_NUM_AC; i++) {
                    if (ar->arAcStreamActive[i]) {
                        if (ar->arAcStreamPriMap[i] > ar->arHiAcStreamActivePri) {
                             
                            ar->arHiAcStreamActivePri = ar->arAcStreamPriMap[i];
                        }
                    }
                }
            }
        }

        AR6000_SPIN_UNLOCK(&ar->arLock, 0);

    } else {
             
        eid = arAc2EndpointID(ar, TrafficClass);
    }

         

    HTCIndicateActivityChange(ar->arHtcTarget,
                              eid,
                              Active);

}

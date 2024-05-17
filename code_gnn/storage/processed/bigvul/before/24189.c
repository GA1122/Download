ar6000_hbChallengeResp_event(struct ar6_softc *ar, u32 cookie, u32 source)
{
    if (source != APP_HB_CHALLENGE) {
         
        if (cookie == ar->arHBChallengeResp.seqNum) {
            ar->arHBChallengeResp.outstanding = false;
        }
    }
}

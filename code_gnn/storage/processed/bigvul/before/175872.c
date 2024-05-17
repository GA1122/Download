int XfrmController::fillUserPolicyId(const XfrmSaInfo& record, XfrmDirection direction,
                                     xfrm_userpolicy_id* usersp) {
    fillXfrmSelector(record, &usersp->sel);
    usersp->dir = static_cast<uint8_t>(direction);
 return sizeof(*usersp);
}

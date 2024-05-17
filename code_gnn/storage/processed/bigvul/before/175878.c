void XfrmController::fillXfrmSelector(const XfrmSaInfo& record, xfrm_selector* selector) {
    selector->family = record.addrFamily;
    selector->proto = AF_UNSPEC;  
}

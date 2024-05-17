int bta_hh_co_write(int fd, UINT8* rpt, UINT16 len)
{
    APPL_TRACE_DEBUG("%s: UHID write %d", __func__, len);

 struct uhid_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_INPUT;
    ev.u.input.size = len;
 if(len > sizeof(ev.u.input.data)){
        APPL_TRACE_WARNING("%s: Report size greater than allowed size",
                           __FUNCTION__);
 return -1;
 }
    memcpy(ev.u.input.data, rpt, len);

 return uhid_write(fd, &ev);

}

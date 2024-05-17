static OMX_ERRORTYPE subscribe_to_events(int fd)
{
    OMX_ERRORTYPE eRet = OMX_ErrorNone;
 struct v4l2_event_subscription sub;
 int array_sz = sizeof(event_type)/sizeof(int);
 int i,rc;
    memset(&sub, 0, sizeof(sub));

 if (fd < 0) {
       DEBUG_PRINT_ERROR("Invalid input: %d", fd);
 return OMX_ErrorBadParameter;
 }

 for (i = 0; i < array_sz; ++i) {
        memset(&sub, 0, sizeof(sub));
        sub.type = event_type[i];
        rc = ioctl(fd, VIDIOC_SUBSCRIBE_EVENT, &sub);

 if (rc) {
           DEBUG_PRINT_ERROR("Failed to subscribe event: 0x%x", sub.type);
 break;
 }
 }

 if (i < array_sz) {
 for (--i; i >=0 ; i--) {
            memset(&sub, 0, sizeof(sub));
            sub.type = event_type[i];
            rc = ioctl(fd, VIDIOC_UNSUBSCRIBE_EVENT, &sub);

 if (rc)
               DEBUG_PRINT_ERROR("Failed to unsubscribe event: 0x%x", sub.type);
 }

        eRet = OMX_ErrorNotImplemented;
 }

 return eRet;
}
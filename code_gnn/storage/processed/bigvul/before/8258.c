SendRange(
    Display 	*dpy,
    XRecordRange **range_item,
    int   	nranges)
{
    int 		rlen = SIZEOF(xRecordRange);
    while(nranges--)
    {
       xRecordRange xrange;

       xrange.coreRequestsFirst = (*range_item)->core_requests.first;
       xrange.coreRequestsLast = (*range_item)->core_requests.last;
       xrange.coreRepliesFirst = (*range_item)->core_replies.first;
       xrange.coreRepliesLast = (*range_item)->core_replies.last;
       xrange.extRequestsMajorFirst = (*range_item)->ext_requests.ext_major.first;
       xrange.extRequestsMajorLast = (*range_item)->ext_requests.ext_major.last;
       xrange.extRequestsMinorFirst = (*range_item)->ext_requests.ext_minor.first;
       xrange.extRequestsMinorLast = (*range_item)->ext_requests.ext_minor.last;
       xrange.extRepliesMajorFirst = (*range_item)->ext_replies.ext_major.first;
       xrange.extRepliesMajorLast = (*range_item)->ext_replies.ext_major.last;
       xrange.extRepliesMinorFirst = (*range_item)->ext_replies.ext_minor.first;
       xrange.extRepliesMinorLast = (*range_item)->ext_replies.ext_minor.last;
       xrange.deliveredEventsFirst = (*range_item)->delivered_events.first;
       xrange.deliveredEventsLast = (*range_item)->delivered_events.last;
       xrange.deviceEventsFirst = (*range_item)->device_events.first;
       xrange.deviceEventsLast = (*range_item)->device_events.last;
       xrange.errorsFirst = (*range_item)->errors.first;
       xrange.errorsLast = (*range_item)->errors.last;
       xrange.clientStarted = (*range_item)->client_started;
       xrange.clientDied = (*range_item)->client_died;

       Data(dpy, (char *)&xrange, rlen);
       range_item++;
    }
}

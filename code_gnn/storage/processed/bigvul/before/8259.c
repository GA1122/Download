WireToLibRange(
    xRecordRange *wire_range,
    XRecordRange *lib_range)
{
    lib_range->core_requests.first = wire_range->coreRequestsFirst;
    lib_range->core_requests.last = wire_range->coreRequestsLast;
    lib_range->core_replies.first = wire_range->coreRepliesFirst;
    lib_range->core_replies.last = wire_range->coreRepliesLast;
    lib_range->ext_requests.ext_major.first = wire_range->extRequestsMajorFirst;
    lib_range->ext_requests.ext_major.last = wire_range->extRequestsMajorLast;
    lib_range->ext_requests.ext_minor.first = wire_range->extRequestsMinorFirst;
    lib_range->ext_requests.ext_minor.last = wire_range->extRequestsMinorLast;
    lib_range->ext_replies.ext_major.first = wire_range->extRepliesMajorFirst;
    lib_range->ext_replies.ext_major.last = wire_range->extRepliesMajorLast;
    lib_range->ext_replies.ext_minor.first = wire_range->extRepliesMinorFirst;
    lib_range->ext_replies.ext_minor.last = wire_range->extRepliesMinorLast;
    lib_range->delivered_events.first = wire_range->deliveredEventsFirst;
    lib_range->delivered_events.last = wire_range->deliveredEventsLast;
    lib_range->device_events.first = wire_range->deviceEventsFirst;
    lib_range->device_events.last = wire_range->deviceEventsLast;
    lib_range->errors.first = wire_range->errorsFirst;
    lib_range->errors.last = wire_range->errorsLast;
    lib_range->client_started = wire_range->clientStarted;
    lib_range->client_died = wire_range->clientDied;
}

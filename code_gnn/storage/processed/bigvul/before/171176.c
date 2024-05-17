status_t MediaPlayerService::Client::invoke(const Parcel& request,
 Parcel *reply)
{
    sp<MediaPlayerBase> p = getPlayer();
 if (p == NULL) return UNKNOWN_ERROR;
 return p->invoke(request, reply);
}

status_t AudioFlinger::EffectHandle::onTransact(
 uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
 return BnEffect::onTransact(code, data, reply, flags);
}

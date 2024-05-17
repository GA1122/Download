AudioFlinger::EffectHandle *AudioFlinger::EffectModule::controlHandle_l()
{
 for (size_t i = 0; i < mHandles.size(); i++) {
 EffectHandle *h = mHandles[i];
 if (h != NULL && !h->destroyed_l()) {
 return h;
 }
 }

 return NULL;
}

bool AudioFlinger::EffectChain::setVolume_l(uint32_t *left, uint32_t *right)
{
 uint32_t newLeft = *left;
 uint32_t newRight = *right;
 bool hasControl = false;
 int ctrlIdx = -1;
 size_t size = mEffects.size();

 for (size_t i = size; i > 0; i--) {
 if (mEffects[i - 1]->isProcessEnabled() &&
 (mEffects[i - 1]->desc().flags & EFFECT_FLAG_VOLUME_MASK) == EFFECT_FLAG_VOLUME_CTRL) {
            ctrlIdx = i - 1;
            hasControl = true;
 break;
 }
 }

 if (!isVolumeForced() && ctrlIdx == mVolumeCtrlIdx &&
 *left == mLeftVolume && *right == mRightVolume) {
 if (hasControl) {
 *left = mNewLeftVolume;
 *right = mNewRightVolume;
 }
 return hasControl;
 }

    mVolumeCtrlIdx = ctrlIdx;
    mLeftVolume = newLeft;
    mRightVolume = newRight;

 if (ctrlIdx >= 0) {
        mEffects[ctrlIdx]->setVolume(&newLeft, &newRight, true);
        mNewLeftVolume = newLeft;
        mNewRightVolume = newRight;
 }
 uint32_t lVol = newLeft;
 uint32_t rVol = newRight;

 for (size_t i = 0; i < size; i++) {
 if ((int)i == ctrlIdx) {
 continue;
 }
 if ((int)i > ctrlIdx) {
            lVol = *left;
            rVol = *right;
 }
        mEffects[i]->setVolume(&lVol, &rVol, false);
 }
 *left = newLeft;
 *right = newRight;

 return hasControl;
}

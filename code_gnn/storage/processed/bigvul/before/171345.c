void OMXCodec::initNativeWindowCrop() {
 int32_t left, top, right, bottom;

    CHECK(mOutputFormat->findRect(
                        kKeyCropRect,
 &left, &top, &right, &bottom));

 android_native_rect_t crop;
    crop.left = left;
    crop.top = top;
    crop.right = right + 1;
    crop.bottom = bottom + 1;

    native_window_set_crop(mNativeWindow.get(), &crop);
}

static void getCopySize(const GifImageDesc& imageDesc, int maxWidth, int maxHeight,
 GifWord& copyWidth, GifWord& copyHeight) {
    copyWidth = imageDesc.Width;
 if (imageDesc.Left + copyWidth > maxWidth) {
        copyWidth = maxWidth - imageDesc.Left;
 }
    copyHeight = imageDesc.Height;
 if (imageDesc.Top + copyHeight > maxHeight) {
        copyHeight = maxHeight - imageDesc.Top;
 }
}

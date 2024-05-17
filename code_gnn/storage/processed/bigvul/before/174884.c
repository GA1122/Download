size_t Camera2Client::calculateBufferSize(int width, int height,
 int format, int stride) {
 switch (format) {
 case HAL_PIXEL_FORMAT_YCbCr_422_SP:  
 return width * height * 2;
 case HAL_PIXEL_FORMAT_YCrCb_420_SP:  
 return width * height * 3 / 2;
 case HAL_PIXEL_FORMAT_YCbCr_422_I:  
 return width * height * 2;
 case HAL_PIXEL_FORMAT_YV12: {  
 size_t ySize = stride * height;
 size_t uvStride = (stride / 2 + 0xF) & ~0xF;
 size_t uvSize = uvStride * height / 2;
 return ySize + uvSize * 2;
 }
 case HAL_PIXEL_FORMAT_RGB_565:
 return width * height * 2;
 case HAL_PIXEL_FORMAT_RGBA_8888:
 return width * height * 4;
 case HAL_PIXEL_FORMAT_RAW_SENSOR:
 return width * height * 2;
 default:
            ALOGE("%s: Unknown preview format: %x",
                    __FUNCTION__,  format);
 return 0;
 }
}

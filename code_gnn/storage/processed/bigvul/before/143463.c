    MediaValuesCached::MediaValuesCachedData createMediaValuesData()
    {
        MediaValuesCached::MediaValuesCachedData data;
        data.viewportWidth = 500;
        data.viewportHeight = 600;
        data.deviceWidth = 700;
        data.deviceHeight = 800;
        data.devicePixelRatio = 2.0;
        data.colorBitsPerComponent = 24;
        data.monochromeBitsPerComponent = 0;
        data.primaryPointerType = PointerTypeFine;
        data.defaultFontSize = 16;
        data.threeDEnabled = true;
        data.mediaType = MediaTypeNames::screen;
        data.strictMode = true;
        data.displayMode = WebDisplayModeBrowser;
         return data;
     }

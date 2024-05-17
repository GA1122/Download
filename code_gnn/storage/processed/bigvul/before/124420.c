void WebRuntimeFeatures::enableBleedingEdgeFastPaths(bool enable)
{
    ASSERT(enable);
    RuntimeEnabledFeatures::setBleedingEdgeFastPathsEnabled(enable);
    RuntimeEnabledFeatures::setSubpixelFontScalingEnabled(enable || RuntimeEnabledFeatures::subpixelFontScalingEnabled());
    RuntimeEnabledFeatures::setWebAnimationsAPIEnabled(enable);
}

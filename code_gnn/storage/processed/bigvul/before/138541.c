static void histogramEnumerationFunction(const char* name, int sample, int boundaryValue)
{
    Platform::current()->histogramEnumeration(name, sample, boundaryValue);
}

cmsTagTypeSignature DecideCurveType(cmsFloat64Number ICCVersion, const void *Data)
{
    cmsToneCurve* Curve = (cmsToneCurve*) Data;

    if (ICCVersion < 4.0) return cmsSigCurveType;
    if (Curve ->nSegments != 1) return cmsSigCurveType;           
    if (Curve ->Segments[0].Type < 0) return cmsSigCurveType;     
    if (Curve ->Segments[0].Type > 5) return cmsSigCurveType;     

    return cmsSigParametricCurveType;
}

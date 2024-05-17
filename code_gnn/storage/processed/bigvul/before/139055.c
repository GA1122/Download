template<> const SVGEnumerationStringEntries& getStaticStringEntries<ColorMatrixType>()
{
    DEFINE_STATIC_LOCAL(SVGEnumerationStringEntries, entries, ());
    if (entries.isEmpty()) {
        entries.append(std::make_pair(FECOLORMATRIX_TYPE_MATRIX, "matrix"));
        entries.append(std::make_pair(FECOLORMATRIX_TYPE_SATURATE, "saturate"));
        entries.append(std::make_pair(FECOLORMATRIX_TYPE_HUEROTATE, "hueRotate"));
        entries.append(std::make_pair(FECOLORMATRIX_TYPE_LUMINANCETOALPHA, "luminanceToAlpha"));
    }
    return entries;
}

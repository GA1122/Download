static GraphicsContext3D::SourceDataFormat getSourceDataFormat(unsigned int componentsPerPixel, AlphaFormat alphaFormat, bool is16BitFormat, bool bigEndian)
{
    const static SourceDataFormatBase formatTableBase[4][AlphaFormatNumFormats] = {  
        { SourceFormatBaseR,          SourceFormatBaseA,          SourceFormatBaseA          },  
        { SourceFormatBaseNumFormats, SourceFormatBaseAR,         SourceFormatBaseRA         },  
        { SourceFormatBaseRGB,        SourceFormatBaseNumFormats, SourceFormatBaseNumFormats },  
        { SourceFormatBaseNumFormats, SourceFormatBaseARGB,       SourceFormatBaseRGBA        }  
    };
    const static GraphicsContext3D::SourceDataFormat formatTable[SourceFormatBaseNumFormats][4] = {  
        { GraphicsContext3D::SourceFormatR8,    GraphicsContext3D::SourceFormatR8,    GraphicsContext3D::SourceFormatR16Little,    GraphicsContext3D::SourceFormatR16Big },
        { GraphicsContext3D::SourceFormatA8,    GraphicsContext3D::SourceFormatA8,    GraphicsContext3D::SourceFormatA16Little,    GraphicsContext3D::SourceFormatA16Big },
        { GraphicsContext3D::SourceFormatAR8,   GraphicsContext3D::SourceFormatRA8,   GraphicsContext3D::SourceFormatRA16Little,   GraphicsContext3D::SourceFormatRA16Big },
        { GraphicsContext3D::SourceFormatRA8,   GraphicsContext3D::SourceFormatAR8,   GraphicsContext3D::SourceFormatAR16Little,   GraphicsContext3D::SourceFormatAR16Big },
        { GraphicsContext3D::SourceFormatBGR8,  GraphicsContext3D::SourceFormatRGB8,  GraphicsContext3D::SourceFormatRGB16Little,  GraphicsContext3D::SourceFormatRGB16Big },
        { GraphicsContext3D::SourceFormatABGR8, GraphicsContext3D::SourceFormatRGBA8, GraphicsContext3D::SourceFormatRGBA16Little, GraphicsContext3D::SourceFormatRGBA16Big },
        { GraphicsContext3D::SourceFormatBGRA8, GraphicsContext3D::SourceFormatARGB8, GraphicsContext3D::SourceFormatARGB16Little, GraphicsContext3D::SourceFormatARGB16Big }
    };

    ASSERT(componentsPerPixel <= 4 && componentsPerPixel > 0);
    SourceDataFormatBase formatBase = formatTableBase[componentsPerPixel - 1][alphaFormat];
    if (formatBase == SourceFormatBaseNumFormats)
        return GraphicsContext3D::SourceFormatNumFormats;
    return formatTable[formatBase][(is16BitFormat ? 2 : 0) + (bigEndian ? 1 : 0)];
}

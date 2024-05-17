dmxProcRenderCompositeGlyphs(ClientPtr client)
{
    int ret;

    REQUEST(xRenderCompositeGlyphsReq);

    ret = dmxSaveRenderVector[stuff->renderReqType] (client);

     
    if (ret == Success) {
        PicturePtr pSrc;
        dmxPictPrivPtr pSrcPriv;
        PicturePtr pDst;
        dmxPictPrivPtr pDstPriv;
        PictFormatPtr pFmt;
        XRenderPictFormat *pFormat;
        int size;

        int scrnNum;
        DMXScreenInfo *dmxScreen;

        CARD8 *buffer;
        CARD8 *end;
        int space;

        int nglyph;
        char *glyphs;
        char *curGlyph;

        xGlyphElt *elt;
        int nelt;
        XGlyphElt8 *elts;
        XGlyphElt8 *curElt;

        GlyphSetPtr glyphSet;
        dmxGlyphPrivPtr glyphPriv;

        dixLookupResourceByType((void **) &pSrc,
                                stuff->src, PictureType, client, DixReadAccess);

        pSrcPriv = DMX_GET_PICT_PRIV(pSrc);
        if (!pSrcPriv->pict)
            return ret;

        dixLookupResourceByType((void **) &pDst,
                                stuff->dst, PictureType,
                                client, DixWriteAccess);

        pDstPriv = DMX_GET_PICT_PRIV(pDst);
        if (!pDstPriv->pict)
            return ret;

        scrnNum = pDst->pDrawable->pScreen->myNum;
        dmxScreen = &dmxScreens[scrnNum];

         
        if (!dmxScreen->beDisplay)
            return ret;

        if (stuff->maskFormat)
            dixLookupResourceByType((void **) &pFmt,
                                    stuff->maskFormat, PictFormatType,
                                    client, DixReadAccess);
        else
            pFmt = NULL;

        pFormat = dmxFindFormat(dmxScreen, pFmt);

        switch (stuff->renderReqType) {
        case X_RenderCompositeGlyphs8:
            size = sizeof(CARD8);
            break;
        case X_RenderCompositeGlyphs16:
            size = sizeof(CARD16);
            break;
        case X_RenderCompositeGlyphs32:
            size = sizeof(CARD32);
            break;
        default:
            return BadPictOp;    
        }

        buffer = (CARD8 *) (stuff + 1);
        end = (CARD8 *) stuff + (stuff->length << 2);
        nelt = 0;
        nglyph = 0;
        while (buffer + sizeof(xGlyphElt) < end) {
            elt = (xGlyphElt *) buffer;
            buffer += sizeof(xGlyphElt);

            if (elt->len == 0xff) {
                buffer += 4;
            }
            else {
                nelt++;
                nglyph += elt->len;
                space = size * elt->len;
                if (space & 3)
                    space += 4 - (space & 3);
                buffer += space;
            }
        }

         

         
        elts = xallocarray(nelt, sizeof(XGlyphElt8));
        if (!elts)
            return BadAlloc;

        glyphs = xallocarray(nglyph, size);
        if (!glyphs) {
            free(elts);
            return BadAlloc;
        }

        buffer = (CARD8 *) (stuff + 1);
        end = (CARD8 *) stuff + (stuff->length << 2);
        curGlyph = glyphs;
        curElt = elts;

        dixLookupResourceByType((void **) &glyphSet,
                                stuff->glyphset, GlyphSetType,
                                client, DixReadAccess);
        glyphPriv = DMX_GET_GLYPH_PRIV(glyphSet);

        while (buffer + sizeof(xGlyphElt) < end) {
            elt = (xGlyphElt *) buffer;
            buffer += sizeof(xGlyphElt);

            if (elt->len == 0xff) {
                dixLookupResourceByType((void **) &glyphSet,
                                        *((CARD32 *) buffer),
                                        GlyphSetType, client, DixReadAccess);
                glyphPriv = DMX_GET_GLYPH_PRIV(glyphSet);
                buffer += 4;
            }
            else {
                curElt->glyphset = glyphPriv->glyphSets[scrnNum];
                curElt->xOff = elt->deltax;
                curElt->yOff = elt->deltay;
                curElt->nchars = elt->len;
                curElt->chars = curGlyph;

                memcpy(curGlyph, buffer, size * elt->len);
                curGlyph += size * elt->len;

                curElt++;

                space = size * elt->len;
                if (space & 3)
                    space += 4 - (space & 3);
                buffer += space;
            }
        }

        switch (stuff->renderReqType) {
        case X_RenderCompositeGlyphs8:
            XRenderCompositeText8(dmxScreen->beDisplay, stuff->op,
                                  pSrcPriv->pict, pDstPriv->pict,
                                  pFormat,
                                  stuff->xSrc, stuff->ySrc, 0, 0, elts, nelt);
            break;
        case X_RenderCompositeGlyphs16:
            XRenderCompositeText16(dmxScreen->beDisplay, stuff->op,
                                   pSrcPriv->pict, pDstPriv->pict,
                                   pFormat,
                                   stuff->xSrc, stuff->ySrc,
                                   0, 0, (XGlyphElt16 *) elts, nelt);
            break;
        case X_RenderCompositeGlyphs32:
            XRenderCompositeText32(dmxScreen->beDisplay, stuff->op,
                                   pSrcPriv->pict, pDstPriv->pict,
                                   pFormat,
                                   stuff->xSrc, stuff->ySrc,
                                   0, 0, (XGlyphElt32 *) elts, nelt);
            break;
        }

        dmxSync(dmxScreen, FALSE);

        free(elts);
        free(glyphs);
    }

    return ret;
}

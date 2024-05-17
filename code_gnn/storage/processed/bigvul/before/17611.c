SProcRenderCompositeGlyphs(ClientPtr client)
{
    xGlyphElt *elt;
    CARD8 *buffer;
    CARD8 *end;
    int space;
    int i;
    int size;

    REQUEST(xRenderCompositeGlyphsReq);
    REQUEST_AT_LEAST_SIZE(xRenderCompositeGlyphsReq);

    switch (stuff->renderReqType) {
    default:
        size = 1;
        break;
    case X_RenderCompositeGlyphs16:
        size = 2;
        break;
    case X_RenderCompositeGlyphs32:
        size = 4;
        break;
    }

    swaps(&stuff->length);
    swapl(&stuff->src);
    swapl(&stuff->dst);
    swapl(&stuff->maskFormat);
    swapl(&stuff->glyphset);
    swaps(&stuff->xSrc);
    swaps(&stuff->ySrc);
    buffer = (CARD8 *) (stuff + 1);
    end = (CARD8 *) stuff + (client->req_len << 2);
    while (buffer + sizeof(xGlyphElt) < end) {
        elt = (xGlyphElt *) buffer;
        buffer += sizeof(xGlyphElt);

        swaps(&elt->deltax);
        swaps(&elt->deltay);

        i = elt->len;
        if (i == 0xff) {
            swapl((int *) buffer);
            buffer += 4;
        }
        else {
            space = size * i;
            switch (size) {
            case 1:
                buffer += i;
                break;
            case 2:
                while (i--) {
                    swaps((short *) buffer);
                    buffer += 2;
                }
                break;
            case 4:
                while (i--) {
                    swapl((int *) buffer);
                    buffer += 4;
                }
                break;
            }
            if (space & 3)
                buffer += 4 - (space & 3);
        }
    }
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}

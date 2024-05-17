bool GIFLZWContext::outputRow(GIFRow::const_iterator rowBegin)
{
    int drowStart = irow;
    int drowEnd = irow;

    if (m_frameContext->progressiveDisplay() && m_frameContext->interlaced() && ipass < 4) {
        unsigned rowDup = 0;
        unsigned rowShift = 0;

        switch (ipass) {
        case 1:
            rowDup = 7;
            rowShift = 3;
            break;
        case 2:
            rowDup = 3;
            rowShift = 1;
            break;
        case 3:
            rowDup = 1;
            rowShift = 0;
            break;
        default:
            break;
        }

        drowStart -= rowShift;
        drowEnd = drowStart + rowDup;

        if (((m_frameContext->height() - 1) - drowEnd) <= rowShift)
            drowEnd = m_frameContext->height() - 1;

        if (drowStart < 0)
            drowStart = 0;

        if ((unsigned)drowEnd >= m_frameContext->height())
            drowEnd = m_frameContext->height() - 1;
    }

    if ((unsigned)drowStart >= m_frameContext->height())
        return true;

    if (!m_client->haveDecodedRow(m_frameContext->frameId(), rowBegin, m_frameContext->width(),
        drowStart, drowEnd - drowStart + 1, m_frameContext->progressiveDisplay() && m_frameContext->interlaced() && ipass > 1))
        return false;

    if (!m_frameContext->interlaced())
        irow++;
    else {
        do {
            switch (ipass) {
            case 1:
                irow += 8;
                if (irow >= m_frameContext->height()) {
                    ipass++;
                    irow = 4;
                }
                break;

            case 2:
                irow += 8;
                if (irow >= m_frameContext->height()) {
                    ipass++;
                    irow = 2;
                }
                break;

            case 3:
                irow += 4;
                if (irow >= m_frameContext->height()) {
                    ipass++;
                    irow = 1;
                }
                break;

            case 4:
                irow += 2;
                if (irow >= m_frameContext->height()) {
                    ipass++;
                    irow = 0;
                }
                break;

            default:
                break;
            }
        } while (irow > (m_frameContext->height() - 1));
    }
    return true;
}

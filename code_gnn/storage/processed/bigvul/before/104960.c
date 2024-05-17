void GraphicsContext::drawLineForTextChecking(const FloatPoint& origin, float width, TextCheckingLineStyle style)
{
    switch (style) {
    case TextCheckingSpellingLineStyle:
        m_data->context->SetPen(wxPen(*wxRED, 2, wxLONG_DASH));
        break;
    case TextCheckingGrammarLineStyle:
        m_data->context->SetPen(wxPen(*wxGREEN, 2, wxLONG_DASH));
        break;
    default:
        return;
    }
    m_data->context->DrawLine(origin.x(), origin.y(), origin.x() + width, origin.y());
 }

Position RenderBlock::positionForBox(InlineBox *box, bool start) const
{
    if (!box)
        return Position();

    if (!box->renderer().nonPseudoNode())
        return createLegacyEditingPosition(nonPseudoNode(), start ? caretMinOffset() : caretMaxOffset());

    if (!box->isInlineTextBox())
        return createLegacyEditingPosition(box->renderer().nonPseudoNode(), start ? box->renderer().caretMinOffset() : box->renderer().caretMaxOffset());

    InlineTextBox* textBox = toInlineTextBox(box);
    return createLegacyEditingPosition(box->renderer().nonPseudoNode(), start ? textBox->start() : textBox->start() + textBox->len());
}

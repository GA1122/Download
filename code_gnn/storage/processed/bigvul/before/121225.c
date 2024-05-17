bool HTMLInputElement::receiveDroppedFiles(const DragData* dragData)
{
    return m_inputType->receiveDroppedFiles(dragData);
}

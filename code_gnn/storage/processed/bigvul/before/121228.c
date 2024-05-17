bool HTMLInputElement::rendererIsNeeded(const NodeRenderingContext& context)
{
    return m_inputType->rendererIsNeeded() && HTMLTextFormControlElement::rendererIsNeeded(context);
}

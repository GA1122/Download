void CSSStyleSheet::styleSheetChanged()
{
    StyleBase* root = this;
    while (StyleBase* parent = root->parent())
        root = parent;
    Document* documentToUpdate = root->isCSSStyleSheet() ? static_cast<CSSStyleSheet*>(root)->doc() : 0;
    
     
    if (documentToUpdate)
        documentToUpdate->updateStyleSelector();
}

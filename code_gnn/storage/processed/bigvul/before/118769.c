static inline bool isInTemplateContent(const Node* node)
{
    Document& document = node->document();
    return &document == document.templateDocument();
}

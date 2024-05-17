HTMLInputElement::HTMLInputElement(const QualifiedName& tagName, Document* document, HTMLFormElement* form, bool createdByParser)
    : HTMLTextFormControlElement(tagName, document, form)
    , m_size(defaultSize)
    , m_maxLength(maximumLength)
    , m_maxResults(-1)
    , m_isChecked(false)
    , m_reflectsCheckedAttribute(true)
    , m_isIndeterminate(false)
    , m_hasType(false)
    , m_isActivatedSubmit(false)
    , m_autocomplete(Uninitialized)
    , m_isAutofilled(false)
    , m_hasNonEmptyList(false)
    , m_stateRestored(false)
    , m_parsingInProgress(createdByParser)
    , m_valueAttributeWasUpdatedAfterParsing(false)
    , m_wasModifiedByUser(false)
    , m_canReceiveDroppedFiles(false)
    , m_hasTouchEventHandler(false)
    , m_inputType(InputType::createText(this))
{
    ASSERT(hasTagName(inputTag) || hasTagName(isindexTag));
#if ENABLE(INPUT_MULTIPLE_FIELDS_UI)
    setHasCustomStyleCallbacks();
#endif
    ScriptWrappable::init(this);
}

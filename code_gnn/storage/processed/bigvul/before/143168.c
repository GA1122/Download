void Document::dataWillChange(const CharacterData& characterData)
{
    if (LocalFrame* frame = this->frame())
        frame->selection().dataWillChange(characterData);
}

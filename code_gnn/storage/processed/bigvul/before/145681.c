PassRefPtrWillBeRawPtr<InputType> ImageInputType::create(HTMLInputElement& element)
{
    return adoptRefWillBeNoop(new ImageInputType(element));
}

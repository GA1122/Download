Tokenizer* ImageDocument::createTokenizer()
{
    return new ImageTokenizer(this);
}

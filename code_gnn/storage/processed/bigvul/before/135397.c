void showLiveDocumentInstances()
{
    Document::WeakDocumentSet& set = Document::liveDocumentSet();
    fprintf(stderr, "There are %u documents currently alive:\n", set.size());
    for (Document* document : set)
        fprintf(stderr, "- Document %p URL: %s\n", document, document->url().string().utf8().data());
}

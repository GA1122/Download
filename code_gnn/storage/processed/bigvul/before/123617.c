static Vector<HTMLImportChild*> importsForFrame(Frame* frame)
{
    Vector<HTMLImportChild*> result;
    Document* rootDocument = frame->document();

    if (HTMLImport* rootImport = rootDocument->import()) {
        for (HTMLImport* import = traverseNext(rootImport); import; import = traverseNext(import)) {
            if (import->ownsLoader() && import->document())
                result.append(toHTMLImportChild(import));
        }
    }

    return result;
}

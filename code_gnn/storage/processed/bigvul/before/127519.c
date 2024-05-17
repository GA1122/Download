bool LayerWebKitThread::filtersCanBeComposited(const FilterOperations& filters)
{
    if (!filters.size())
        return false;

    for (unsigned i = 0; i < filters.size(); ++i) {
        const FilterOperation* filterOperation = filters.at(i);
        switch (filterOperation->getOperationType()) {
        case FilterOperation::REFERENCE:
#if ENABLE(CSS_SHADERS)
        case FilterOperation::CUSTOM:
#endif
            return false;
        default:
            break;
        }
    }

    return true;
}

static SearchCandidate searchTreeForFindClosestLayoutSVGText(LayoutObject* layoutObject, const FloatPoint& point)
{
    SearchCandidate closestText;
    Vector<SearchCandidate> candidates;

    for (LayoutObject* child = layoutObject->slowLastChild(); child; child = child->previousSibling()) {
        if (child->isSVGText()) {
            float distance = distanceToChildLayoutObject(child, point);
            if (distance >= closestText.candidateDistance)
                continue;
            candidates.clear();
            closestText.candidateLayoutObject = child;
            closestText.candidateDistance = distance;
            continue;
        }

        if (child->isSVGContainer() && !layoutObject->isSVGHiddenContainer()) {
            float distance = distanceToChildLayoutObject(child, point);
            if (distance > closestText.candidateDistance)
                continue;
            candidates.append(SearchCandidate(child, distance));
        }
    }

    if (closestText.candidateLayoutObject && candidates.isEmpty())
        return closestText;

    std::stable_sort(candidates.begin(), candidates.end(), compareCandidateDistance);

    for (const SearchCandidate& searchCandidate : candidates) {
        if (closestText.candidateDistance < searchCandidate.candidateDistance)
            break;
        LayoutObject* candidateLayoutObject = searchCandidate.candidateLayoutObject;
        FloatPoint candidateLocalPoint = candidateLayoutObject->localToSVGParentTransform().inverse().mapPoint(point);

        SearchCandidate candidateText = searchTreeForFindClosestLayoutSVGText(candidateLayoutObject, candidateLocalPoint);

        if (candidateText.candidateDistance < closestText.candidateDistance)
            closestText = candidateText;
    }

    return closestText;
}

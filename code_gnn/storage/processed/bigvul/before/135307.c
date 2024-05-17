static LayoutObject* nearestCommonHoverAncestor(LayoutObject* obj1, LayoutObject* obj2)
{
    if (!obj1 || !obj2)
        return 0;

    for (LayoutObject* currObj1 = obj1; currObj1; currObj1 = currObj1->hoverAncestor()) {
        for (LayoutObject* currObj2 = obj2; currObj2; currObj2 = currObj2->hoverAncestor()) {
            if (currObj1 == currObj2)
                return currObj1;
        }
    }

    return 0;
}

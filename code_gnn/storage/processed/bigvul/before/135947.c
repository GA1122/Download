bool ContainerNode::getUpperLeftCorner(FloatPoint& point) const
{
    if (!layoutObject())
        return false;

    LayoutObject* o = layoutObject();
    if (!o->isInline() || o->isReplaced()) {
        point = o->localToAbsolute(FloatPoint(), UseTransforms);
        return true;
    }

    while (o) {
        LayoutObject* p = o;
        if (LayoutObject* oFirstChild = o->slowFirstChild()) {
            o = oFirstChild;
        } else if (o->nextSibling()) {
            o = o->nextSibling();
        } else {
            LayoutObject* next = nullptr;
            while (!next && o->parent()) {
                o = o->parent();
                next = o->nextSibling();
            }
            o = next;

            if (!o)
                break;
        }
        ASSERT(o);

        if (!o->isInline() || o->isReplaced()) {
            point = o->localToAbsolute(FloatPoint(), UseTransforms);
            return true;
        }

        if (p->node() && p->node() == this && o->isText() && !o->isBR() && !toLayoutText(o)->hasTextBoxes()) {
        } else if ((o->isText() && !o->isBR()) || o->isReplaced()) {
            point = FloatPoint();
            if (o->isText() && toLayoutText(o)->firstTextBox()) {
                point.move(toLayoutText(o)->linesBoundingBox().x(), toLayoutText(o)->firstTextBox()->root().lineTop().toFloat());
                point = o->localToAbsolute(point, UseTransforms);
            } else if (o->isBox()) {
                LayoutBox* box = toLayoutBox(o);
                point.moveBy(box->location());
                point = o->container()->localToAbsolute(point, UseTransforms);
            }
            return true;
        }
    }

    if (!o && document().view()) {
        point = FloatPoint(0, document().view()->contentsHeight());
        return true;
    }
    return false;
}

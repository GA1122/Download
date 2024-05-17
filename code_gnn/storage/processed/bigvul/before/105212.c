void replaceChildrenWithText(ContainerNode* container, const String& text, ExceptionCode& ec)
{
    RefPtr<ContainerNode> containerNode(container);

#if ENABLE(MUTATION_OBSERVERS)
    ChildListMutationScope mutation(containerNode.get());
#endif

    if (hasOneTextChild(containerNode.get())) {
        toText(containerNode->firstChild())->setData(text, ec);
        return;
    }

    RefPtr<Text> textNode = Text::create(containerNode->document(), text);

    if (hasOneChild(containerNode.get())) {
        containerNode->replaceChild(textNode.release(), containerNode->firstChild(), ec);
        return;
    }

    containerNode->removeChildren();
    containerNode->appendChild(textNode.release(), ec);
}

void replaceChildrenWithFragment(ContainerNode* container, PassRefPtr<DocumentFragment> fragment, ExceptionCode& ec)
{
    RefPtr<ContainerNode> containerNode(container);

#if ENABLE(MUTATION_OBSERVERS)
    ChildListMutationScope mutation(containerNode.get());
#endif

    if (!fragment->firstChild()) {
        containerNode->removeChildren();
        return;
    }

    if (hasOneTextChild(containerNode.get()) && hasOneTextChild(fragment.get())) {
        toText(containerNode->firstChild())->setData(toText(fragment->firstChild())->data(), ec);
        return;
    }

    if (hasOneChild(containerNode.get())) {
        containerNode->replaceChild(fragment, containerNode->firstChild(), ec);
        return;
    }

    containerNode->removeChildren();
    containerNode->appendChild(fragment, ec);
}

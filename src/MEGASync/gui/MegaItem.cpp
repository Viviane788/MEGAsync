#include "MegaItem.h"

#include <QByteArray>

using namespace mega;

MegaItem::MegaItem(MegaNode *node, MegaItem *parentItem, bool showFiles)
{
    this->node = node;
    this->children = NULL;
    this->parent = parentItem;
    this->showFiles = showFiles;
}

mega::MegaNode *MegaItem::getNode()
{
    return node;
}

void MegaItem::setChildren(MegaNodeList *children)
{
    this->children = children;
    for (int i = 0; i < children->size(); i++)
    {
        MegaNode *node = children->get(i);
        if (!showFiles && node->getType() == MegaNode::TYPE_FILE)
        {
            break;
        }
        childItems.append(new MegaItem(children->get(i), this, showFiles));
    }
}

bool MegaItem::areChildrenSet()
{
    return children != NULL;
}

MegaItem *MegaItem::getParent()
{
    return parent;
}

MegaItem *MegaItem::getChild(int i)
{
    return childItems.at(i);
}

int MegaItem::getNumChildren()
{
    return childItems.size();
}

int MegaItem::indexOf(MegaItem *item)
{
    return childItems.indexOf(item);
}

int MegaItem::insertPosition(MegaNode *node)
{
    int type = node->getType();

    int i;
    for (i = 0; i < childItems.size(); i++)
    {
        MegaNode *n = childItems.at(i)->getNode();
        int nodeType = n->getType();
        if (type < nodeType)
        {
            continue;
        }

        if (qstricmp(node->getName(), n->getName()) <= 0)
        {
            break;
        }
    }

    return i;
}

void MegaItem::insertNode(MegaNode *node, int index)
{
    childItems.insert(index, new MegaItem(node, this, showFiles));
    insertedNodes.append(node);
}

void MegaItem::removeNode(MegaNode *node)
{
    if (!node)
    {
        return;
    }

    for (int i = 0; i < childItems.size(); i++)
    {
        if (childItems[i]->getNode()->getHandle() == node->getHandle())
        {
            delete childItems[i];
            childItems.removeAt(i);
            break;
        }
    }

    for (int i = 0; i < insertedNodes.size(); i++)
    {
        if (insertedNodes[i]->getHandle() == node->getHandle())
        {
            delete insertedNodes[i];
            insertedNodes.removeAt(i);
            break;
        }
    }
}

void MegaItem::displayFiles(bool enable)
{
    this->showFiles = enable;
}

MegaItem::~MegaItem()
{
    delete children;
    qDeleteAll(childItems);
    qDeleteAll(insertedNodes);
}


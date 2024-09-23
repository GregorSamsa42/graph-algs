//  Algorithm deciding if two rooted trees are isomorphic
//  Authors: Georǵi Kocharyan

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <memory>

// Node has to store parent to make naming possible in linear time

struct TreeNode
{
    std::vector<std::unique_ptr<TreeNode> > children;
    std::vector<int> name;
    TreeNode *parent;

    TreeNode() = default;

    explicit TreeNode(TreeNode *node) : parent(node)
    {
    }

    void add_child()
    {
        children.push_back(std::make_unique<TreeNode>(this));
    }

    bool operator<(TreeNode const &other) const
    {
        if (name.size() != other.name.size()) {
            return name.size() < other.name.size() ? true : false;
        }
        for (size_t i = 0; i < name.size(); i++) {
            if (name[i] != other.name[i]) {
                return name[i] < other.name[i] ? true : false;
            }
        }
        return false;
    }
};

// outputs a vector of lists of each depth of the tree

std::vector<std::list<TreeNode *> > tree_levels(TreeNode *root)
{
    if (!root) return {};

    std::vector<std::list<TreeNode *> > result;
    std::queue<std::pair<TreeNode *, int> > q;
    q.emplace(root, 0);

    while (!q.empty()) {
        TreeNode *node = q.front().first;
        const int depth = q.front().second;
        q.pop();

        // If this is a new depth, add a new list to the result
        if (depth >= result.size()) {
            result.emplace_back();
        }

        // Add the current node's value to the appropriate depth list
        result[depth].push_back(node);

        // Enqueue children with their depth
        for (const auto &child: node->children) {
            q.emplace(child.get(), depth + 1);
        }
    }
    return result;
}

bool lexicographicOrder(const TreeNode *a, const TreeNode *b)
{
    return (*a) < (*b);
}

// to do: actually implement linear bucket sort
void bucketSort(std::list<TreeNode *> &nodes)
{
    if (nodes.empty()) return;
    nodes.sort(lexicographicOrder);
}

void tree_isomorphism(TreeNode *root, TreeNode *root2)
{
    std::vector<std::list<TreeNode *> > levels1 = tree_levels(root);
    std::vector<std::list<TreeNode *> > levels2 = tree_levels(root2);

    const int height = levels1.size() - 1;

    // if heights are different trees cannot be isomorphic
    if (!(height == levels2.size() - 1)) {
        std::cout << "Trees are not isomorphic. Their heights are different." << std::endl;
        return;
    }

    // the name of all leaves is 1
    const std::vector<int> leaf_name = {1};

    for (auto leaf: levels1[height]) {
        leaf->name = leaf_name;
    }
    for (const auto leaf: levels2[height]) {
        leaf->name = leaf_name;
    }

    for (int i = height - 1; i >= 0; i--) {
        const int amount = levels1[i].size();
        if (amount != levels2[i].size()) {
            std::cout << "Trees are not isomorphic, they have a different number of nodes in depth " << i << "." <<
                    std::endl;
            return;
        }

        // name all nodes of current height by iterating through all (sorted) children

        for (const auto &node: levels1[i + 1]) {
            node->parent->name.insert(node->parent->name.end(), node->name.begin(), node->name.end());
        }

        for (const auto &node: levels2[i + 1]) {
            node->parent->name.insert(node->parent->name.end(), node->name.begin(), node->name.end());
        }

        // sort nodes of current height by name
        bucketSort(levels1[i]);
        bucketSort(levels2[i]);

        // check if both trees have the same names in the current level
        auto itr1 = levels1[i].begin();
        auto itr2 = levels2[i].begin();

        for (auto _ = amount; _--;) {
            if (((*itr1)->name) != ((*itr2)->name)) {
                std::cout << "Trees are not isomorphic!" << std::endl;
                return;
            }
            ++itr1;
            ++itr2;
        }

        // rename words according to index in sorted vector
        int count = 0;
        for (const auto & node: levels1[i]) {
            node->name = {count++};
            count++;
        }
        count = 0;
        for (const auto & node: levels2[i]) {
            node->name = {count++};
        }
    }

    std::cout << "Trees are isomorphic." << std::endl;
}

std::unique_ptr<TreeNode> create_tree1()
{
    auto root = std::make_unique<TreeNode>();
    root->add_child();
    root->add_child();
    root->add_child();
    root->children[1]->add_child();
    root->children[1]->add_child();
    root->children[2]->add_child();
    root->children[1]->children[1]->add_child();
    root->children[2]->children[0]->add_child();
    root->children[2]->children[0]->add_child();
    return root;
}

std::unique_ptr<TreeNode> create_tree2()
{
    auto root = std::make_unique<TreeNode>();
    root->add_child();
    root->add_child();
    root->add_child();
    root->children[0]->add_child();
    root->children[2]->add_child();
    root->children[2]->add_child();
    root->children[0]->children[0]->add_child();
    root->children[2]->children[1]->add_child();
    root->children[2]->children[1]->add_child();
    return root;
}

int main()
{
    const auto root1 = create_tree1();
    const auto root2 = create_tree2();

    tree_isomorphism(root1.get(), root2.get());

    return 0;
}

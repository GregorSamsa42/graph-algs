//  Algorithm deciding if two rooted trees are isomorphic
//  Authors: Georǵi Kocharyan

#include <iostream>
#include <cstdio>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <string>

// Node has to store parent to make naming possible in linear time

struct TreeNode {
    std::vector<TreeNode*> children;
    std::string name;
    TreeNode* parent;
    TreeNode() = default;
    TreeNode(TreeNode* node) : parent(node) {}
    void add_child()
    {
        children.push_back(new TreeNode(this));
    }
};

// outputs a vector of lists of each depth of the tree

std::vector<std::list<TreeNode*>> tree_levels(TreeNode* root) {
   if (!root) return {};

    std::vector<std::list<TreeNode*>> result;
    std::queue<std::pair<TreeNode*, int>> q;
    q.push(std::make_pair(root, 0));

    while (!q.empty())
    {
        TreeNode* node = q.front().first;
        int depth = q.front().second;
        q.pop();

        // If this is a new depth, add a new list to the result
        if (depth >= result.size()) {
            result.push_back(std::list<TreeNode*>());
        }

        // Add the current node's value to the appropriate depth list
        result[depth].push_back(node);

        // Enqueue children with their depth
        for (const auto& child : node->children) {
            q.push(std::make_pair(child, depth + 1));
        }
    }
    return result;
}
    
bool lexicographicOrder(TreeNode* a,TreeNode* b) {
    return (a->name)<(b->name);
}

// to do: actually implement linear bucket sort
void bucketSort(std::list<TreeNode*>& nodes) {
    if (nodes.empty()) return;
    nodes.sort(lexicographicOrder);
}

int main() {

    // initialise tree 1
    TreeNode* root = new TreeNode();
    root->add_child();
    root->add_child();
    root->add_child();
    root->children[1]->add_child();
    root->children[1]->add_child();
    root->children[2]->add_child();
    root->children[1]->children[1]->add_child();
    root->children[2]->children[0]->add_child();
    root->children[2]->children[0]->add_child();

    // initialise tree 2
    TreeNode* root2 = new TreeNode();
    root2->add_child();
    root2->add_child();
    root2->add_child();
    root2->children[0]->add_child();
    root2->children[2]->add_child();
    root2->children[2]->add_child();
    root2->children[0]->children[0]->add_child();
    root2->children[2]->children[1]->add_child();
    root2->children[2]->children[1]->add_child();

    // store all nodes by their depth
    std::vector<std::list<TreeNode*>> levels1 = tree_levels(root);
    std::vector<std::list<TreeNode*>> levels2 = tree_levels(root2);

    int height = levels1.size()-1;

    // if heights are different trees cannot be isomorphic
    if (!(height == levels2.size()-1)) 
    {
        std::cout << "Trees are not isomorphic. Their heights are different." << std::endl;
        return 0;
    }

    // the name of all leaves is 1
    std::string s("1");

    for (auto leaf : levels1[height])
    {
        leaf->name = s;
    }
    for (auto leaf : levels2[height])
    {
        leaf->name = s;
    }

    for (int i = height-1; i >= 0; i--)
    {
        int amount = levels1[i].size();
        if (!(amount == levels2[i].size()))
        {
            std::cout << "Trees are not isomorphic, they have a different number of nodes in depth " << i << "." << std::endl;
            return 0;
        }

        // name all nodes of current height by iterating through all (sorted) children

        for (const auto& node : levels1[i+1])
        {
            node->parent->name = node->parent->name + node->name;
        }

        for (const auto& node : levels2[i+1])
        {
            node->parent->name = node->parent->name + node->name;
        }

        // sort nodes of current height by name
        bucketSort(levels1[i]);
        bucketSort(levels2[i]);

        // check if both trees have the same names in the current level
        std::list<TreeNode*>::iterator itr1 = levels1[i].begin();
        std::list<TreeNode*>::iterator itr2 = levels2[i].begin();

        for (auto _ = amount; _--;)
        {
            if (!(((*itr1)->name) == ((*itr2)->name)))
            {
                std::cout << "Trees are not isomorphic!" << std::endl;
                return 0;
            }
            itr1++;
            itr2++;
        }

        // rename words according to index in sorted vector
        int count = 0;
        for (auto& node : levels1[i])
        {
            node->name = std::to_string(count);
            count++;
        }
        count = 0;
        for (auto& node : levels2[i])
        {
            node->name = std::to_string(count);
            count++;
        }
    }
        
    std::cout << "Trees are isomorphic." << std::endl;
    return 0;
}
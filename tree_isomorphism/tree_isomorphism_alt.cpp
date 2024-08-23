//  Algorithm deciding if two rooted trees are isomorphic
//  Authors: Georǵi Kocharyan

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <queue>

// Node has to store parent to make naming possible in linear time

struct TreeNode {
    std::list<TreeNode> children;
    std::vector<int> name;
    TreeNode* parent;
    TreeNode() = default;
    TreeNode(TreeNode* node) : parent(node) {}
    void add_child()
    {
        children.emplace_back(this);
    }
    bool operator<(TreeNode const & other) const 
    {
        if (name.size() != other.name.size())
        {
            return name.size() < other.name.size() ? true : false;
        }
        for (size_t i = 0; i < name.size(); i++) 
        {
            if (name[i] != other.name[i])
            {
                return name[i] < other.name[i] ? true : false;
            }
        }
        return false;
    } 
};

bool compareTreeNodes(const TreeNode* a, const TreeNode* b)
{
    return *a < *b;
}

// outputs a vector of lists of each depth of the tree

std::vector<std::list<TreeNode*>> tree_levels(TreeNode & root) {

    std::vector<std::list<TreeNode*>> result;
    std::queue<std::pair<TreeNode*, int>> q;
    q.push(std::make_pair(&root, 0));

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
        for (auto & child : node->children) {
            q.push(std::make_pair(&child, depth + 1));
        }
    }
    {

    }
    return result;
}

// to do: actually implement linear bucket sort
void bucketSort(std::list<TreeNode*> & nodes) {
    if (nodes.empty()) return;
    nodes.sort(compareTreeNodes);
}

void tree_isomorphism(TreeNode & root, TreeNode & root2) {
    // store all nodes by their depth
    std::vector<std::list<TreeNode*>> levels1 = tree_levels(root);
    std::vector<std::list<TreeNode*>> levels2 = tree_levels(root2);

    int height = levels1.size()-1;

    // if heights are different trees cannot be isomorphic
    if (!(height == levels2.size()-1)) 
    {
        std::cout << "Trees are not isomorphic. Their heights are different." << std::endl;
        return;
    }

    // the name of all leaves is 1
    std::vector<int> s(1,1);

    for (auto leaf : levels1[height])
    {
        leaf->name = s;
    }
    for (auto leaf : levels2[height])
    {
        leaf->name = s;
    }

    for (int i = height; i >= 1; i--)
    {
        int amount = levels1[i].size();
        // std::cout << amount << " " << levels2[i].size() << std::endl;
        if (!(amount == levels2[i].size()))
        {
            std::cout << "Trees are not isomorphic, they have a different number of nodes in depth " << i << "." << std::endl;
            return;
        }

        if (i == height) {
            continue;
        }

        // name all nodes of current height by iterating through all (sorted) children

        for (const auto node : levels1[i+1])
        {
            if (node && node-> parent)
            {
                node->parent->name.insert(node->parent->name.end(), node->name.begin(), node->name.end());
            }
        }

        for (const auto node : levels2[i+1])
        {
            if (node && node-> parent)
            {
                (node->parent)->name.insert((node->parent)->name.end(), node->name.begin(), node->name.end());
            }
        }

        // sort nodes of current height by name
        bucketSort(levels1[i]);
        bucketSort(levels2[i]);

        // check if both trees have the same names in the current level
        std::list<TreeNode*>::iterator itr1 = levels1[i].begin();
        std::list<TreeNode*>::iterator itr2 = levels2[i].begin();
        std::cout << amount << std::endl;
        for (int j = 0; j < amount; j++)
        {
            for (auto i : (*itr1)->name) {
                std::cout << i << std::endl;
            }
            if ((*itr2)->name != ((*itr1)->name))
            {
                std::cout << "Trees are not isomorphic!" << std::endl;
                return;
            }
            itr1++;
            itr2++;
        }

        // rename words according to index in sorted vector
        int count = 0;
        for (auto& node : levels1[i])
        {
            std::vector<int> position(count,1);
            node->name = position;
            count++;
        }
        count = 0;
        for (auto& node : levels2[i])
        {
            std::vector<int> position(count,1);
            node->name = position;
            count++;
        }
    }
        
    std::cout << "Trees are isomorphic." << std::endl;
}

int main() {

    // initialise tree 1
    TreeNode root;
    root.add_child();
    root.add_child();
    root.add_child();
    std::list<TreeNode>::iterator itr = root.children.begin();
    itr++;
    itr->add_child();
    itr->add_child();
    std::list<TreeNode>::iterator itr1 = itr->children.begin();
    itr1->add_child();
    itr++;
    itr->add_child();
    itr->children.begin()->add_child();
    itr->children.begin()->add_child();

    // initialise tree 2
    TreeNode root2;
    root2.add_child();
    root2.add_child();
    root2.add_child();
    itr = root2.children.begin();
    itr->add_child();
    itr->children.begin()->add_child();
    itr++;
    itr++;
    itr->add_child();
    itr->add_child();
    itr1 = itr->children.begin();
    itr1++;
    itr1->add_child();
    itr1->add_child();

    tree_isomorphism(root, root2);
    
    return 0;
}
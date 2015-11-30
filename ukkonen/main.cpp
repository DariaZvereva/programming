#include <iostream>
#include <map>
#include <memory>

struct Node;

struct Position {
    std::shared_ptr<Node> finish;
    long long distFromFinish;

    Position(): finish(nullptr), distFromFinish(0) {}

    Position(std::shared_ptr<Node> f, size_t d): finish(f), distFromFinish(d) {};
};

struct Node {
    std::shared_ptr<Node> parent;//родитель
    std::shared_ptr<Node> suffixLink;//суффиксная ссылка
    std::map <char, Position> children;//дети
    long long parentEdgeSubstringStart; //включая первый символ
    long long parentEdgeSubstringFinish;//не включая последний

    Node (std::shared_ptr<Node> p):  parent(p), suffixLink(nullptr), parentEdgeSubstringFinish(0), parentEdgeSubstringStart(0) {};

    Node(std::shared_ptr<Node> p, long long s, long long f): parent(p), suffixLink(nullptr), parentEdgeSubstringFinish(f), parentEdgeSubstringStart(s) {};

    long long getLength() {
        return parentEdgeSubstringFinish - parentEdgeSubstringStart;
    }
};


class SuffixTree {
private:

    std::shared_ptr<Node> blank;
    std::shared_ptr<Node> root;
    std::string strForBuildingTree;
    Position currentPosition;
    long long curEndOfStr;
    long long nodes;

    //символ, следующий за текущей позицией
    char getSymbolNextToPosition(Position pos) {

        return strForBuildingTree[pos.finish->parentEdgeSubstringFinish - pos.distFromFinish];
    }

    Position getSuffixLink(Position pos) {
        std::shared_ptr<Node> currentNode = pos.finish->parent->suffixLink;
        if (pos.distFromFinish == 0 && pos.finish->suffixLink != nullptr) {
            //мы в вершине
            return Position(pos.finish->suffixLink, 0);
        }

        long long left = pos.finish->parentEdgeSubstringStart;
        long long right = pos.finish->parentEdgeSubstringFinish - pos.distFromFinish;

        while (left < right) {
            char ch = strForBuildingTree[left];
            //если длина ребра, начинающегося с ch равна длине подстроки, соответствующей рассматриваемому отрезку, переходим в потомка
            if (currentNode->children[ch].finish->getLength() == right - left) {
                return Position(currentNode->children[ch].finish, 0);
            } else {
                //если длина ребра, начинающегося с ch меньше длины подстроки, соответствующей рассматриваемому отрезку, переходим в потомка
                if (currentNode->children[ch].finish->getLength() < right - left) {
                    left += currentNode->children[ch].finish->getLength();
                    currentNode = currentNode->children[ch].finish;
                } else {
                    //длина ребра больше - просто переходим
                    return Position(currentNode->children[ch].finish,
                                    currentNode->children[ch].finish->getLength() - (right - left));
                }
            }
        }
    }


    std::shared_ptr<Node> splitEdge(Position &pos) {
        if (pos.distFromFinish == 0) {
            return pos.finish;
        }

        std::shared_ptr<Node> newNode = std::shared_ptr<Node>(new Node(pos.finish->parent));
        newNode->parentEdgeSubstringStart = pos.finish->parentEdgeSubstringStart;
        newNode->parentEdgeSubstringFinish = pos.finish->parentEdgeSubstringFinish - pos.distFromFinish;
        newNode->children.insert(
                std::pair<char, Position>(getSymbolNextToPosition(pos), Position(pos.finish, pos.distFromFinish - 1)));
        pos.finish->parent->children[strForBuildingTree[pos.finish->parentEdgeSubstringStart]] = Position(newNode, newNode->getLength() - 1);
        pos.finish->parent = newNode;
        pos.finish->parentEdgeSubstringStart = pos.finish->parentEdgeSubstringFinish - pos.distFromFinish;
        pos = Position(newNode, 0);
        return newNode;
    }

    bool canGo(Position &pos, char ch) {
        if (pos.distFromFinish != 0) {
            if  (getSymbolNextToPosition(pos) == ch)
                return true;
        } else {
            if (pos.finish->children.find(ch) != pos.finish->children.end())
                return true;
        }
        return false;
    }

    void createLeaf(std::shared_ptr<Node> node, char ch) {
        std::shared_ptr<Node> newLeaf = std::make_shared<Node>(node);
        newLeaf->parentEdgeSubstringStart = curEndOfStr;
        newLeaf->parentEdgeSubstringFinish = strForBuildingTree.size();
        node->children.insert(std::pair<char, Position>(ch, Position(newLeaf, newLeaf->getLength() - 1)));
    }

    void go(char ch) {
        //on the edge
        if (currentPosition.distFromFinish != 0) {
            currentPosition.distFromFinish--;
            return;
        }
        //in a node
        currentPosition = currentPosition.finish->children[ch];
    }

    void addLetter() {
        char ch = strForBuildingTree[curEndOfStr];

        while (!canGo(currentPosition, ch)){
            std::shared_ptr<Node> newNode = splitEdge(currentPosition);
            createLeaf(newNode, ch);


            Position suffixLinkPosition = getSuffixLink(currentPosition);
            newNode->suffixLink = splitEdge(suffixLinkPosition);

            currentPosition = Position(newNode->suffixLink, 0);
        }
        go(ch);
        curEndOfStr++;

    }

    long long dfs(std::shared_ptr<Node> node) {
        long long ans = 0;
        if (node->children.empty()) {
            return node->getLength();
        }
        ans += node->getLength();
        for (auto iter: node->children){
            ans += dfs(iter.second.finish);
        }
        return ans;
    }


public:

    //Initialization
    SuffixTree(std::string &s): currentPosition(nullptr, 0) {
        strForBuildingTree = s;
        curEndOfStr = 0;
        blank = std::shared_ptr<Node>(new Node(nullptr));
        blank->suffixLink = blank;
        root = std::shared_ptr<Node>(new Node(blank));
        root->suffixLink = blank;
        root->parentEdgeSubstringFinish = 0;
        root->parentEdgeSubstringStart = -1;
        nodes = 2;

        for (long long i = 0; i < strForBuildingTree.length(); ++i) {
            blank->children.insert(std::pair<char, Position>(s[i], Position(root, 0)));
        }
        currentPosition = Position(root, 0);
        for (long long i = 0; i < strForBuildingTree.length(); ++i){
            addLetter();
        }
    }

    long long countSubstr() {
        return dfs(root);
    }
};

int main()
{
    std::string s;
    std::cin >> s;
    SuffixTree Tree(s);
    std::cout << Tree.countSubstr() - 1 << std::endl;//без пустой строки
    return 0;
}

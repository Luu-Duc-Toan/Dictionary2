#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stack>
#include<assert.h>
#include<string>
#include<vector>
#include<queue>
#define MAXSUGGESTION 5
using namespace std;
int maxKeySize = 0;

struct Node {
    string key;
    string meaning;
    Node* pLeft;
    Node* pRight;
    int height;

    Node(string key, string meaning) {
        this->key = key;
        this->meaning = meaning;
        pLeft = NULL;
        pRight = NULL;
        height = 1;
    }

    ~Node() {
        if(!pLeft) delete pLeft;
        pLeft = NULL;
        if(!pRight) delete pRight;
        pRight = NULL;
    }
};

void demolishTree(Node*& pRoot) {
    if(pRoot == NULL) return;
    delete pRoot; 
    pRoot = NULL;
}
void UpdateHeight(Node* &pRoot) {
	if(pRoot == NULL) return;
    int leftHeight = pRoot->pLeft == NULL ? 0 : pRoot->pLeft->height;
    int rightHeight = pRoot->pRight == NULL ? 0 : pRoot->pRight->height;
	pRoot->height = max(leftHeight, rightHeight) + 1;
}
void LeftRotate(Node*& pRoot) {
    Node *y = pRoot->pRight;
    pRoot->pRight = y->pLeft;
    y->pLeft = pRoot;
    pRoot = y;
    UpdateHeight(y->pLeft);
    UpdateHeight(y);
}
void RightRotate(Node*& pRoot) {
    Node* y = pRoot->pLeft;
    pRoot->pLeft = y->pRight;
    y->pRight = pRoot;
    pRoot = y;
    UpdateHeight(y->pRight);
    UpdateHeight(y);
}
void Balance(Node*& pRoot) {
    if (!pRoot) return;
    int leftHeight = pRoot->pLeft == NULL ? 0 : pRoot->pLeft->height;
    int rightHeight = pRoot->pRight == NULL ? 0 : pRoot->pRight->height;
    if(abs(leftHeight - rightHeight) < 2) return;
    if (rightHeight > leftHeight) {
        int rightLeftHeight = pRoot->pRight->pLeft == NULL ? 0 : pRoot->pRight->pLeft->height;
        int rightRightHeight = pRoot->pRight->pRight == NULL ? 0 : pRoot->pRight->pRight->height;
        if (rightLeftHeight > rightRightHeight) {
            RightRotate(pRoot->pRight);
            LeftRotate(pRoot);
        }
        else {
            LeftRotate(pRoot);
        }
    }
    else {
        int leftLeftHeight = pRoot->pLeft->pLeft == NULL ? 0 : pRoot->pLeft->pLeft->height;
        int leftRightHeight = pRoot->pLeft->pRight == NULL ? 0 : pRoot->pLeft->pRight->height;
        if (leftRightHeight > leftLeftHeight) {
            LeftRotate(pRoot->pRight);
            RightRotate(pRoot);
        }
        else {
            RightRotate(pRoot);
        }
    }

}
//bor
int Comparator(string& s1, string& s2) {
    int n1 = s1.size();
    int n2 = s2.size();
    if(n1 < n2) return -1;
    if(n1 > n2) return 1;
    for (int i = 0; i < n1; i++) {
        if (s1[i] < s2[i]) return -1;
        else if (s1[i] > s2[i]) return 1;
    }
    return 0;
}
void Insert(Node*& pRoot, string &key, string &meaning) {
    if(pRoot == NULL) {
		pRoot = new Node(key, meaning);
		return;
	}
    if(Comparator(pRoot->key, key) == 0) 
        return;
    else if(Comparator(pRoot->key, key) < 0) 
        Insert(pRoot->pRight, key, meaning);
	else
        Insert(pRoot->pLeft, key, meaning);
    UpdateHeight(pRoot);
    Balance(pRoot);
}


void SwapWithMaxSuc(Node*& pRoot, Node*& suc) {
    if (!suc->pRight) {
        pRoot->key = suc->key;
        Node* tmp = suc;
        suc = suc->pLeft;
        delete tmp;
    }
	else {
		SwapWithMaxSuc(pRoot, suc->pRight);
        UpdateHeight(suc);
        Balance(suc);
	}
}
void Remove(Node*& pRoot, string &key) {
    if (pRoot == NULL) {
        return;
    }
    if (Comparator(pRoot->key, key) == 0) {
        if (pRoot->pRight && pRoot->pLeft) {
            SwapWithMaxSuc(pRoot, pRoot->pLeft);
        }
        else if (pRoot->pRight) {
            Node* tmp = pRoot;
            pRoot = pRoot->pRight;
            tmp->pRight = NULL;
            delete tmp;
        }
        else if (pRoot->pLeft) {
            Node* tmp = pRoot;
            pRoot = pRoot->pLeft;
            tmp->pLeft = NULL;
            delete tmp;
        }
        else {
            delete pRoot;
			pRoot = NULL;
        }
    }
    else if (Comparator(pRoot->key, key) < 0) 
        Remove(pRoot->pRight, key);
    else 
        Remove(pRoot->pLeft, key);
    UpdateHeight(pRoot);
    Balance(pRoot);
}

bool isAVL(Node* pRoot) {
    if (pRoot == NULL) return true;
    int leftHeight = pRoot->pLeft == NULL ? 0 : pRoot->pLeft->height;
    int rightHeight = pRoot->pRight == NULL ? 0 : pRoot->pRight->height;
    if (abs(leftHeight - rightHeight) > 1) return false;
    return isAVL(pRoot->pLeft) && isAVL(pRoot->pRight);
}

int height(Node* pRoot) {
    return pRoot->height;
}

void LRN(Node* &pRoot) {
      if (pRoot == NULL) return;
      LRN(pRoot->pLeft);
      LRN(pRoot->pRight);
      cout << pRoot->key << " ";
}

void NLR(Node* pRoot) {
    if (pRoot == NULL) return;
    cout << pRoot->key << " ";
    NLR(pRoot->pLeft);
    NLR(pRoot->pRight);
}

void LNR(Node* pRoot) {
    if (pRoot == NULL) return;
    LNR(pRoot->pLeft);
    cout << pRoot->key << " ";
    LNR(pRoot->pRight);
}

void LevelOrder(Node* pRoot) {
    if (pRoot == NULL) return;
    queue<Node*> q;
    q.push(pRoot);
    while (!q.empty()) {
        Node *cur = q.front();
        q.pop();
        cout << cur->key << " ";
        if (cur->pLeft) q.push(cur->pLeft);
        if (cur->pRight) q.push(cur->pRight);
    }
}

Node* createTreeFromArray(vector<pair<string, string>>& arr) {
    Node* pRoot = NULL;
    for (int i = 0; i < arr.size(); ++i) {
        maxKeySize = max(maxKeySize, (int)arr[i].first.size());
        Insert(pRoot, arr[i].first, arr[i].second);
    }
    return pRoot;
}
void Zfunction(vector<int>& z, string& key) {
    int n = key.size();
    z = vector<int>(n, 0);
    int l = 0;
    int r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r) {
            z[i] = min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && key[i + z[i]] == key[z[i]]) {
            z[i]++;
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
}
bool CheckKey(string& inputKey, string& key) {
    string s = key + "#" + inputKey;
    vector<int> z(s.size());
    Zfunction(z, s);
    int n1 = inputKey.size();
    for (auto& size : z) {
        if (size == n1) return true;
    }
    return false;
}
Node* SameKeySizeTree(Node*& root, string& inputKey) {
    int n = inputKey.size();
    Node* cur = root;
    while (cur->key.size() == n) {
        if (cur->key.size() > n) {
            cur = cur->pRight;
        }
        else {
            cur = cur->pLeft;
        }
    }
    return cur;
}
void CheckKeys(Node*& node, string& inputKey, vector<string>& suggestions) {
    if (suggestions.size() == MAXSUGGESTION) return;
    if (node == NULL) return;
    if (node->key.size() >= inputKey.size()) {
        if (CheckKey(inputKey, node->key)) {
            suggestions.push_back(node->key);
        }
    }
    CheckKeys(node->pLeft,inputKey, suggestions);
    CheckKeys(node->pRight, inputKey, suggestions);
}
vector<string> Suggest(Node*& root, string& inputKey) {
    if (inputKey.size() > maxKeySize) return vector<string>();
    Node* startNode = SameKeySizeTree(root, inputKey);
    vector<string> suggestions;
    CheckKeys(startNode, inputKey, suggestions);
    return suggestions;
}

int main() {
    freopen("input.txt", "r+", stdin);
    freopen("output.txt", "w+", stdout);

    vector<int> z1;
    string inputKey = "na";
    int n, k; cin >> n; cin.ignore();
    vector<pair<string, string>> arr(n); 
    for (auto& i : arr) {
        getline(cin, i.first);
        getline(cin, i.second);
    }

    Node* root = createTreeFromArray(arr);
    assert(isAVL(root));
    LNR(root);

    //While
    vector<string> suggestions = Suggest(root, inputKey);
    for (auto& sug : suggestions) {
        cout << sug << endl;
    }

    demolishTree(root);
    assert(root == NULL);

    return 0;
}
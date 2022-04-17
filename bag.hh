#ifndef _BAG_HH
#define _BAG_HH

#include <vector>
using namespace std;

class pennant {
public:
    pennant(): _root(nullptr), _size(0){}
    pennant(int val): _root(new node), _size(1){ _root->_val = val; }
    operator bool() const { return _size > 0; }
    unsigned size() const { return _size; }
    pennant union_with(pennant src) {
        _size += src._size;
        if (!_root) {
            _root = src._root;
        } else {
            src._root->_right = _root->_left;
            _root->_left = src._root;
        }
        return *this;
    }
    pennant split() {
        pennant dst;
        dst._root = _root->_left;
        _root->_left = dst._root->_right;
        dst._root->_right = nullptr;
        dst._size = _size / 2;
        _size /= 2;
        return dst;
    }
    vector<int> content() {
        vector<int> ans;
        if (_root) dfs(_root, ans);
        return ans;
    }
private:
    struct node {
        int _val;
        node *_left, *_right;
    };
    node *_root;
    unsigned _size;
    void dfs(node *now, vector<int> &ans) {
        ans.push_back(now->_val);
        if (now->_left) dfs(now->_left, ans);
        if (now->_right) dfs(now->_right, ans);
    }
};

class bag {
public:
    static const unsigned BAGNUM = 30;
    void insert(pennant x) {
        unsigned k;
        for (k = 0; _item[k]; ++k) {
            x = _item[k].union_with(x);
            _item[k] = pennant();
        }
        _item[k] = x;
    }
    void union_with(bag src) {
        pennant carry;
        for (unsigned k = 0; k < BAGNUM; ++k) {
            if (!_item[k] && src._item[k] && !carry) {
                _item[k] = src._item[k];
            } else if (!_item[k] && !src._item[k] && carry) {
                _item[k] = carry;
                carry = pennant();
            } else if (_item[k] && src._item[k] && !carry) {
                carry = _item[k].union_with(src._item[k]);
                _item[k] = pennant();
            } else if (_item[k] && !src._item[k] && carry) {
                carry = _item[k].union_with(carry);
                _item[k] = pennant();
            } else if (src._item[k] && carry) {
                carry = src._item[k].union_with(carry);
            }
        }
    }
    unsigned size() const {
        unsigned ans = 0;
        for (unsigned k = 0; k < BAGNUM; ++k)
            ans += _item[k].size();
        return ans;
    }
    pennant &operator [](int k) { return _item[k]; }
private:
    pennant _item[BAGNUM];
};

#endif
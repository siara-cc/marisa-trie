#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <memory>
#include <unordered_set>
#include <set>

using namespace std;

class node {
  public:
    node *first_child;
    node *parent;
    node *next_sibling;
    string val;
    bool is_leaf;
    int level;
    node() {
      first_child = parent = next_sibling = NULL;
      is_leaf = false;
      level = 0;
    }
};

enum {SRCH_ST_UNKNOWN, SRCH_ST_NEXT_SIBLING, SRCH_ST_NOT_FOUND, SRCH_ST_NEXT_CHAR};

node *root = new node();
int node_count = 0;
vector<node *> allnodes;
//int oct_node_count = 1;
set<string> all_keys;

// typedef struct {
//   node *to_insert_at;
//   int val_pos;
//   int key_pos;
//   int state;
// } search_ret;

// search_ret search(const string& key) {
//   node *trav_node = root->children[0].get();
//   node *prev_node = trav_node;
//   int key_pos = 0;
//   do {
//     int val_pos = 0;
//     int key_left = key.length() - key_pos;
//     int val_len = trav_node->val->length();
//     int len_to_cmp = (key_left > val_len ? val_len : key_left);
//     int sw = SRCH_ST_UNKNOWN;
//     do {
//         sw = (trav_node->val->at(val_pos) < key[key_pos] ? SRCH_ST_NEXT_SIBLING : (trav_node->val->at(val_pos) > key[key_pos] ? SRCH_ST_NOT_FOUND : SRCH_ST_NEXT_CHAR));
//         if (sw == SRCH_ST_NEXT_CHAR)
//           key_pos++;
//         val_pos++;
//     } while (sw == SRCH_ST_NEXT_CHAR && --len_to_cmp);
//     switch (sw) {
//       case SRCH_ST_NEXT_CHAR:
//         key_left -= len_to_cmp;
//         if (key_left == 0)
//           return (search_ret) {trav_node, val_pos, key_pos, sw};
//         trav_node = trav_node->children[0].get();
//         break;
//       case SRCH_ST_NEXT_SIBLING:
//         prev_node = trav_node;
//         trav_node = trav_node->next_sibling.get();
//         break;
//       case SRCH_ST_NOT_FOUND:
//         search_ret ret({trav_node, 0, key_pos, sw});
//         return ret;
//     }
//   } while (trav_node != NULL);
//   return (search_ret) {prev_node, 0, key_pos, SRCH_ST_NOT_FOUND};
// }

node *next(node *from, string& out_str) {
  if (from == root) {
    do {
      from = from->first_child;
    } while (!from->is_leaf);
  } else {
    if (from->first_child == NULL) {
      while (from != NULL && from->next_sibling == NULL)
        from = from->parent;
      if (from == NULL)
        return root;
      from = from->next_sibling;
      while (!from->is_leaf)
        from = from->first_child;
    } else {
      do {
        from = from->first_child;
      } while (!from->is_leaf);
    }
  }
  node *cur_node = from;
  out_str.clear();
  while (cur_node != root) {
    out_str.insert(0, cur_node->val[0] == '\n' ? cur_node->val.substr(1) : cur_node->val);
    cur_node = cur_node->parent;
  }
  return from;
}

node *last_child(node *node) {
  node = node->first_child;
  while (node != NULL && node->next_sibling != NULL)
    node = node->next_sibling;
  return node;
}

void append_existing(node *new_node, vector<string>& existing) {
  if (existing.size() > 0) {
    new_node->is_leaf = false;
    node *parent = new_node;
    for (int i = 0; i < existing.size(); i++) {
      node *child = new node();
      parent->first_child = child;
      child->level = parent->level + 1;
      child->parent = parent;
      child->val = existing[i];
      child->is_leaf = false;
      parent = child;
  allnodes.push_back(child);
      node_count++;
    }
    parent->is_leaf = true;
  } else
    new_node->is_leaf = true;
}

void append_key(const string& key, vector<string>& existing) {
  int key_pos = 0;
  node *last_sibling = last_child(root);
  do {
    const string *val = &last_sibling->val;
    int i = 0;
    for (; i < val->length(); i++) {
      if (key[key_pos] != val->at(i)) {
        if (i == 0) {
          node *new_node = new node();
          new_node->level = last_sibling->level;
          new_node->parent = last_sibling->parent;
          new_node->next_sibling = NULL;
          new_node->val = key.substr(key_pos);
          append_existing(new_node, existing);
          //if (new_node->val == string(" discuss"))
          //  cout << "New node: " << key << endl;
  allnodes.push_back(new_node);
          last_sibling->next_sibling = new_node;
          node_count++;
          //if ((last_sibling->val[0] >> 3) != (new_node->val[0] >> 3))
          //  oct_node_count++;
        } else {
          node *child1 = new node();
          node *child2 = new node();
          child1->is_leaf = last_sibling->is_leaf;
          child1->level = last_sibling->level + 1;
          child1->parent = last_sibling;
          child1->next_sibling = child2;
          child1->val = val->substr(i);
          //if (child1->val == string(" discuss"))
          //  cout << "Child1 node: " << key << endl;
          if (last_sibling->first_child != NULL) {
            node *node = last_sibling->first_child;
            child1->first_child = node;
            do {
              node->parent = child1;
              node->level++;
              node = node->next_sibling;
            } while (node != NULL);
          }
  allnodes.push_back(child1);
          child2->level = last_sibling->level + 1;
          child2->parent = last_sibling;
          child2->next_sibling = NULL;
          child2->val = key.substr(key_pos);
          append_existing(child2, existing);
          //if (child2->val == string(" discuss"))
          //  cout << "Child2 node: " << key << endl;
          last_sibling->first_child = child1;
          last_sibling->is_leaf = false;
          node_count += 2;
          //oct_node_count += ((child1->val[0] >> 3) == (child2->val[0] >> 2) ? 1 : 3);
  allnodes.push_back(child2);
          last_sibling->val.erase(i);
        }
        return;
      }
      key_pos++;
    }
    if (i == val->length() && key_pos < key.length()
        && last_sibling->is_leaf && last_sibling->first_child == NULL) {
      node *child1 = new node();
      child1->level = last_sibling->level + 1;
      child1->parent = last_sibling;
      child1->next_sibling = NULL;
      child1->val = key.substr(key_pos);
      append_existing(child1, existing);
      //    if (child1->val == string(" discuss"))
      //      cout << "Ext node: " << key << endl;
      last_sibling->first_child = child1;
      node_count++;
      //oct_node_count++;
  allnodes.push_back(child1);
      return;
    }
    last_sibling = last_child(last_sibling);
  } while (last_sibling != NULL);
}

int compare(string v1, string v2) {
    int k = 0;
    int len1 = v1.length();
    int len2 = v2.length();
    int lim = (len2 < len1 ? len2 : len1);
    while (k < lim) {
        unsigned char c1 = v1[k];
        unsigned char c2 = v2[k];
        k++;
        if (c1 < c2)
            return -k;
        else if (c1 > c2)
            return k;
    }
    if (len1 == len2)
        return 0;
    k++;
    return (len1 < len2 ? -k : k);
}

vector<string> find_existing(string key) {
  vector<string> ret;
  string not_found;
  not_found.append(1, key[0]);
  key.erase(0, 1);
  while (key.length() > 0) {
    set<string>::iterator it = all_keys.lower_bound(key);
    int cret = abs(compare(key, *it));
    if (cret == 1 && it != all_keys.begin()) {
      --it;
      cret = abs(compare(key, *it));
    }
    if (abs(cret) > 10 || (cret == 0 && key.length() > 8)) {
      if (not_found.length() > 0) {
        ret.push_back(not_found);
        not_found.clear();
      }
      string s = "\n";
      s.append(key.substr(0, cret-1));
      ret.push_back(s);
      key.erase(0, cret-1);
    } else {
      not_found.append(1, key[0]);
      key.erase(0, 1);
    }
  }
  if (not_found.length() > 0)
    ret.push_back(not_found);
  return ret;
}

int main(int argc, char *argv[]) {

  fstream infile;
  infile.open(argv[1], ios::in);
  int line_count = 0;
  if (infile.is_open()) {
    string line;
    string prev_line = "";
    while (getline(infile, line)) {
      if (line == prev_line)
         continue;
      all_keys.insert(line);
      prev_line = line;
      line_count++;
      if ((line_count % 100000) == 0) {
        cout << ".";
        cout.flush();
      }
    }
  }
  infile.close();
  cout << "Inserted into set" << endl;

  line_count = 0;  
  root->parent = NULL;
  root->next_sibling = NULL;
  root->is_leaf = false;
  root->level = 0;
  node *first_node = new node();
  first_node->parent = root;
  first_node->next_sibling = NULL;
  first_node->level = 1;
  allnodes.push_back(first_node);
  root->first_child = first_node;
  infile.open(argv[1], ios::in);
  if (infile.is_open()) {
    string line;
    string prev_line = "";
    while (getline(infile, line)) {
      if (line == prev_line)
         continue;
      vector<string> existing;
      if (argc > 2) {
        existing.push_back(line);
      } else
        existing = find_existing(line);
      if (node_count == 0) {
        first_node->val = existing[0];
        existing.erase(existing.begin());
        append_existing(first_node, existing);
        node_count++;
      } else {
        string key = existing[0];
        existing.erase(existing.begin());
        append_key(key, existing);
      }
      line_count++;
      if ((line_count % 1000) == 0) {
        cout << ".";
        cout.flush();
      }
      prev_line = line;
    }
  }
  infile.close();

  int tail1_count = 0;
  int tail_size = 0;
  int existing_count = 0;
  int existing_size = 0;
  int max_tail_len = 0;
  unordered_set<string> tail_set;
  unordered_set<string> existing_set;
  for (int i = 0; i < allnodes.size(); i++) {
    if (allnodes[i]->val.length() > 1) {
      string val = allnodes[i]->val;
      if (val[0] == '\n') {
        if (tail_set.find(val.substr(1)) == tail_set.end()) {
          existing_count++;
          existing_size += val.size();
          existing_size--;
          existing_set.insert(val.substr(1));
        }
      } else
      if (tail_set.find(val) == tail_set.end()) {
        if (argc < 3) {
          vector<string> existing = find_existing(val);
          if (existing.size() > 1) {
            existing_count += existing.size();
            existing_count--;
            for (int j = 0; j < existing.size(); j++)
              existing_size += existing[j].length();
          }
        }
        tail_set.insert(val);
        tail_size += val.length();
        if (val.length() > max_tail_len)
          max_tail_len = val.length();
        cout << val << endl;
      }
    } else
      tail1_count++;
  }

  cout << "Total lines: " << line_count << endl;
  cout << "Node count: " << node_count << endl;
  //cout << "Oct Node count: " << oct_node_count << endl;
  cout << "Tail1 Count: " << tail1_count << endl;
  cout << "Tail Ptr Count: " << (node_count - tail1_count) << endl;
  cout << "Unique tail count: " << tail_set.size() << endl;
  cout << "Unique tails size: " << tail_size << endl;
  cout << "Max tail len: " << max_tail_len << endl;
  cout << "Existing count: " << existing_count << endl;
  cout << "Existing size: " << existing_size << endl;

                // trie_chars   bitmaps          pointers                                tails
  int total_size = node_count + node_count / 2 + (node_count - tail1_count) * (ceil(log2(tail_size))-8)/8 + tail_size + tail_set.size();

  cout << "Estimated size: " << total_size << endl;
/*
  node *cur_node = root;
  do {
    string next_str;
    cur_node = next(cur_node, next_str);
    cout << next_str << endl;
  } while (cur_node != root);
*/
}

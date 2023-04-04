#include "rbtree.h"
#include <stdio.h> 
#include <stdlib.h>



rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));

  nilNode->color = RBTREE_BLACK;
  p->nil = nilNode;
  p->root = p->nil;

  return p;
}
void right_rotation(rbtree * tree, node_t * x){
    // TODO!

    node_t *y;

    // 1. target의 left으로 y 설정
    y = x->left;
    // 2. y의 오른쪽 서브트리를 target의 왼쪽 서브트리로 옮김
    x->left = y->right;
    // 3. y의 오른쪽 노드가 NIL이 아니라면, y의 오른쪽 노드 부모를 target으로 설정
    if (y->right != tree->nil) {
        y->right->parent = x;
    }
    // 4. y의 부모 노드를 target의 부모 노드로 설정
    y->parent = x->parent;
    // 5. target의 부모 노드가 nil이라면, 트리 구조체의 root를 y로 설정
    if (x->parent == tree->nil)
        tree->root = y;
    // 6. target이 target 부모 노드의 왼쪽이면, target 부모의 왼쪽을 y로 설정
    else if (x == x->parent->left)
        x->parent->left = y;
    // 7. target이 target 부모 노드의 오른쪽이면, target 부모의 오른쪽을 y로 설정
    else
        x->parent->right = y;
    // 8. target을 y의 오른쪽으로 설정
    y->right = x;
    // 9. target의 부모를 y로 설정
    x->parent = y;
}

void left_rotation(rbtree * tree, node_t * x){
    // TODO!
    node_t *y;

    y = x->right;
    x->right = y->left;
    
    if (y->left != tree->nil) 
        y->left->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == tree->nil) 
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else 
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t *y;
  
  while (z->parent->color == RBTREE_RED) {
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    if (z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;
      
      // CASE 1 : 노드 z의 삼촌 y가 적색인 경우
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // CASE 2 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotation(t, z);
        }
        // CASE 3 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotation(t, z->parent->parent);
      }
    }
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    else {
      y = z->parent->parent->left;

      // CASE 4 : 노드 z의 삼촌 y가 적색인 경우
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // CASE 5 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotation(t, z);
        }
        // CASE 6 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotation(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

// 기존 Binary Search Tree의 Insert에서 변형
// 하지만 삽입할 자리를 찾는 과정은 동일하다. NULL이 nil 노드로 바뀐다는 점과, 새로 삽입할 노드는 항상 단말 노드가 되기 때문에 새로 삽입할 노드 z의 left, right가 NIL이 된다는 점이 다르다.
// 또한 새로 삽입할 노드 z가 레드 블랙 트리의 색깔 특성을 위반시킬 수 있으므로 따로 함수(rbtree_insert_fixup)를 호출하여 보완해줄 필요가 있다.
// insert하고자 하는 위치를 찾고자 최악의 경우 log n이 걸릴 수 있다.
// 그리고 while이나 for문 안에서 fixup이 이루어지는 것이 아니라 그냥 삽입이 끝나고 재조정을 위해 1번 이루어진다.
// 따라서 (log n)^2가 아니라 log n + log n = 2log n으로, 삽입의 시간 복잡도는 log n이다.

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: 노드 삽입할 위치 찾기
  node_t* x = t->root; //node being compared with Key
  node_t* y = t->nil; // y will be parent of key
  node_t* newNode = (node_t*)malloc(sizeof(node_t));
  newNode->key = key;
  

  while(x != t->nil){ //descend until reaching the sentinel
    y = x;
    if (newNode->key < x->key){
      x = x->left;
    }
    else
      x = x->right;
  }
 
  
  newNode->parent = y; //found the location-insert "newNode->parent" with parent y 

  if (y == t->nil){

    t->root = newNode; //tree T was empty
  }
  else if (newNode->key < y->key)
  {
    y->left = newNode;
  }
  else{
    y->right = newNode; //both of newNode's children are the sentinel
  }

  newNode->left = newNode->right = t->nil;
  newNode->color = RBTREE_RED; //the new node startsout red
  rbtree_insert_fixup(t, newNode);
  //correct any violations of red-black proprties
  


  
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t* p =  t->root;
  int check = 0;


  while(check == 0){ 
    //p가 닐인지 부터 확인하기
    if (p == t->nil){
      return NULL;
    }
    
    if (p->key == key){

      check = 1;
      return p;
    }
    else if (p->key > key){
      p = p->left;

    }
    else if (p->key < key) {
      p = p->right;
    }
    else { 
      check = 1;
      }
  }
 return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  
  // TODO: implement to_array
  return 0;
}



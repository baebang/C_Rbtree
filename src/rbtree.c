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
  //처음 루트가 들어왔을때
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
  //밸런스 맞춰주기
  
  
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
  node_t *p = t->root;
  while(p->left != t->nil){
    p = p->left;
    
  }

  return p;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
   node_t *p = t->root;
  while(p->right != t->nil){
    p = p->right;
    
  }

  return p;
}

// [이식] 
// 빠진 노드를 하단 노드에 이식하는 과정임
// 걍 쉽게 말해서 u자리에 v를 넣는 과정
void rbtree_transplant(rbtree *t, node_t *u,node_t *v){
  if(u->parent == t->nil){
    //u가 루트인지 확인해봐
    //맞으면 v를 루트로 바꾸삼
    t->root = v; 
  }
  else if(u == u->parent->left){
    //근데 루트 아니고 왼쪽에 있는 노드야
    u->parent->left = v;
    //그 부모 노드를 v노드로 바꿔
  }
  else{
    //같은말임
    u->parent->right = v;
  }
}

void free_node(rbtree *t, node_t *x) {
  // 후위 순회 방식으로 RB Tree 내의 노드 메모리 반환
  if (x->left != t->nil) 
    free_node(t, x->left);
  if (x->right != t->nil)
    free_node(t, x->right);
  free(x);
  x = NULL;
}

// [동적 메모리 할당 해제]
// [동적 메모리 할당된 트리의 nil 노드와 트리의 메모리 할당 해제]
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil)
    free_node(t, t->root);
  free(t->nil);
  free(t);
}

int rbtree_erase(rbtree *t, node_t *z) {
  // // TODO: implement erase
  // node_t *y= z; //지우는 값 
  // color_t y_origin_color = y->color; 
  // // 나중에 y의 color, 즉 우리가 삭제하고자 하는 노드에 들어갈(대체할), 실제로 삭제하는 노드의 색깔이다. 
  // // 이는 레드 블랙 트리의 삭제 원리에 따라 블랙일 경우 문제가 되기에 미리 저장해둔다. 

  // if(z->left == t->nil){
  //   x = a->right;
  //   rbtree_transplant(t,z,z->right);
  // }
  // else if(z->right == t->nil){
  //   x = z->left;
  //   rbtree_transplant(t,z,z->left);
  // }
  // else{
  //   y = rbtree_min(z->right);
  // }


  return 0;
}

// [중위 순회]
void inorder_rbtree_to_array(const rbtree *t, node_t *x, key_t *arr, int *idx, const size_t n) {
  node_t *stack[n];
  int top = 0;

  while (x != t->nil || top > 0) {
    while (x != t->nil) {
      stack[top++] = x;
      x = x->left;
    }

    x = stack[--top];

    if (*idx < n) {
      arr[(*idx)++] = x->key;
    } else {
      return;
    }

    x = x->right;
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *x = t->root;
  if (x == t->nil) {
    return 0;
  }
  int cnt = 0;
  int *idx = &cnt;
  inorder_rbtree_to_array(t, x, arr, idx, n);
  
  // TODO: implement to_array
  return 0;
}

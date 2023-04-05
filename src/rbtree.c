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
  p->root->left = p->nil;
  p->root->right = p->nil;


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

void rbtree_erase_fixup(rbtree *t, node_t *x){
    node_t *w;                                          // x의 형제 노드를 가리키는 w 포인터를 미리 선언한다.
  
  while (x != t->root && x->color == RBTREE_BLACK) {  // x가 root가 되면 단순히 그냥 검은색으로 바꾸면된다. 그리고 while문 아래는 x가 doubly black일 떄 이뤄진다.
    // doubly black인 x가 왼쪽 자식일 때
    if (x == x->parent->left) {
      w = x->parent->right;
      
      // <경우 1> : 경우 2, 3, 4로 위임 가능
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotation(t, x->parent);
        w = x->parent->right;                         // 회전을 끝내고 난 후에는 x->parent->right가 새로운 노드가 되고 밑의 if, else if, else 중 한 가지, 즉 경우 2, 3, 4의 한 가지로 위임된다.
      }
      
      // 위의 if문을 만나지 않았으므로, w->color == RBTREE_BLACK인 경우이다.
      // <경우 2> : 경우 1, 2, 3, 4로 위임 가능
      // x->parent로 짬 때리는 경우이다.
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;                        // x의 extra black을 x->parent로 넘긴다. 그러면서 w는 red가 된다.
        x = x->parent;                                // 새롭게 doubly black 혹은 red and black이 x->parent이 짬 맞아서 재조정을 진행하도록 위임한다.
      } else {
        
        // <경우 3> : 경우 4로 위임 가능
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotation(t, w);
          w = x->parent->right;
        }
        
        // <경우 4> : 특성이 위반되는 것을 해결한다. 경우 4는 다른 경우로 위임되지 않고 위반을 해결(특성을 만족)한다.
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotation(t, x->parent);
        x = t->root;                                  // 경우 4를 거치면 특성 위반이 해결되는 것이므로 x를 root로 설정하여 while문을 빠져나가도록 한다.
      }
    
    // doubly black인 x가 오른쪽 자식일 때
    } else {
      w = x->parent->left;
      
      // <경우 1>
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation(t, x->parent);
        w = x->parent->left;
      }
      
      // <경우 2>
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        
        // <경우 3>
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotation(t, w);
          w = x->parent->left;
        }
        
        // <경우 4>
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotation(t, x->parent);
        x = t->root;
      }
    }
  }
  
  x->color = RBTREE_BLACK;        
}

node_t *rbtree_min(const rbtree *t){
    // TODO: implement find
  if (t->root == t->nil) {
    return NULL;
  }
  node_t * curr = t->root;

  while (curr->left != t->nil) {
    curr = curr->left;
  }
  return curr;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil) {
    return NULL;
  }

  node_t * curr = t->root;

  while (curr->right != t->nil) {
    curr = curr->right;
  }
  return curr;
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
  // TODO: implement erase
  node_t *y= z; //지우는 값 
  color_t y_origin_color = y->color; 
  // 나중에 y의 color, 즉 우리가 삭제하고자 하는 노드에 들어갈(대체할), 실제로 삭제하는 노드의 색깔이다. 
  // 이는 레드 블랙 트리의 삭제 원리에 따라 블랙일 경우 문제가 되기에 미리 저장해둔다.
  node_t *x; 

  if(z->left == t->nil){
    // 오른쪽에 자식이 있을때
    x = z->right;
    rbtree_transplant(t,z,z->right);
  }
  else if(z->right == t->nil){
    // 왼쪽에 자식이 있을때
    x = z->left;
    rbtree_transplant(t,z,z->left);
  }
  else{
    // 자식이 있을때
    y = rbtree_min(z->right); // successor을 뽑자
    y_origin_color = y->color; // successor의 색상으로 변경
    x = y->right; // 자식이 있는지 확인하기 위함
    if(y != z->right){ // is farther down the tree?
      rbtree_transplant(t,y,y->right); //하위 노드가 있다면 바꿔바꿔
      y->right = z->right;
      y->right->parent = y;

    }
    else{
      x->parent = y;
      rbtree_transplant(t,z,y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }

  
  }

  if (y_origin_color == RBTREE_BLACK) {
    rbtree_erase_fixup(t, x);   
    // 대체하는 노드 y의 자식인 x가 y의 자리로 올라오면서 
    // 이 x에 extra black을 부여한다. 이 extra black을 처리하면서 재조정하는 것이 fixup의 원리이므로 x를 인자로 넣는다.
  }

  free(z);
  return 0;
}

// [중위 순회]
void inorder_rbtree_to_array(const rbtree *t, node_t *x, key_t *arr, int *idx, const size_t n) {
  // 스택으로 사용할 배열을 생성하고, 스택의 맨 위(top)를 0으로 초기화
  node_t *stack[n];
  int top = 0;
  // 모든 노드를 처리하거나 배열이 가득 찰 때까지 반복
  while (x != t->nil || top > 0) {
    // 현재 노드의 모든 왼쪽 자식 노드를 스택에 추가
    while (x != t->nil) {
      stack[top++] = x;
      x = x->left;
    }
    // 스택에서 현재 노드를 팝
    x = stack[--top];
    
    

    // 배열에 공간이 있으면 현재 노드의 키를 배열에 추가
    // 그렇지 않으면 함수를 종료
    if (*idx < n) {
      arr[(*idx)++] = x->key;
    } else {
      return;
    }

    // 현재 노드의 오른쪽 자식 노드를 처리
    x = x->right;
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *x = t->root;
  // 루트 노드가 존재하지 않으면 배열에 아무것도 채우지 않고 0을 반환
  if (x == t->nil) {
    return 0;
  }
  // 배열에 삽입된 키의 개수를 저장할 변수 cnt 초기화
  int cnt = 0;
  // 포인터 cnt_ptr이 cnt 변수의 주소를 가리키도록 함
  // inorder_rbtree_to_array 함수에서 cnt 변수를 참조할 수 있도록 하기 위함!
  int *cnt_ptr = &cnt;
  // 중위 순회 함수 호출을 통해 트리의 모든 노드의 키 값을 배열에 삽입
  inorder_rbtree_to_array(t, x, arr, cnt_ptr, n);
  
  // TODO: implement to_array
  return 0;
}


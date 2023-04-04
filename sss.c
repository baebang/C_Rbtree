// [삽입_insert]
// 기존 Binary Search Tree의 Insert에서 변형
// 하지만 삽입할 자리를 찾는 과정은 동일하다. NULL이 nil 노드로 바뀐다는 점과, 새로 삽입할 노드는 항상 단말 노드가 되기 때문에 새로 삽입할 노드 z의 left, right가 NIL이 된다는 점이 다르다.
// 또한 새로 삽입할 노드 z가 레드 블랙 트리의 색깔 특성을 위반시킬 수 있으므로 따로 함수(rbtree_insert_fixup)를 호출하여 보완해줄 필요가 있다.
// insert하고자 하는 위치를 찾고자 최악의 경우 log n이 걸릴 수 있다.
// 그리고 while이나 for문 안에서 fixup이 이루어지는 것이 아니라 그냥 삽입이 끝나고 재조정을 위해 1번 이루어진다.
// 따라서 (log n)^2가 아니라 log n + log n = 2log n으로, 삽입의 시간 복잡도는 log n이다.
node_t *rbtree_insert(rbtree *t, const key_t key) {   // rbtree만큼의 크기를 가르키는 포인터 t와, 새로운 삽입할 노드의 키 값을 매개 변수로 받는다.
  node_t *x = t->root;                                // 삽입할 곳을 탐색해 나가는 포인터로, nil(NULL)을 찾으면 x를 삽입한다.
  node_t *y = t->nil;                                 // parent를 저장해두는 포인터이다. 우리는 이미 parent라는 변수를 사용하고 있으므로, 해당 포인터의 변수명을 y로 정했다.
  
  // 중복되는 값도 넣을 수 있어야 한다. 여기서는 오른쪽에 넣는 것으로 동작한다.
  while (x != t->nil) {                               // nil이 아니라면 계속 찾고, nil이라면 root부터 비어있다는 뜻이다. 그리고 while문을 한 번도 돌지 않기 때문에 x는 주소값으로 t->nil을 가지고 있게 된다.
    y = x;                                            // y에는 미리 x의 값(주소값)을 저장해놓는다. parent를 저장해놓기 위해서이다.
    if (x->key > key) {                               // 삽입하려는 값이 현재 x가 가르키는 노드의 키 값보다 작다면 왼쪽으로 탐색한다.
      x = x->left;
    } else {                                          // 삽입하려는 값이 현재 x가 가르키는 노드의 키 값보다 크다면 오른쪽으로 탐색한다.
      x = x->right;                                   // 추가로, 만약 (x->key == key)를 만족하여 중복되는 값이 있다면 else문을 만나 오른쪽에 넣도록 동작하게 된다.
    }
  }
  
  // while 문을 다 돌게 되면 x는 nil을 가르키게 되었다는 뜻으로, parent인 y에 새로운 노드인 z를 삽입하면 된다.
  // 새로운 노드 z 생성(할당)
  node_t *z = (node_t *)calloc(1, sizeof(node_t));    // node_t만큼 메모리에 동적으로 할당시키고 이 공간의 시작 주소를 포인터 z에 대입한다.                       
  z->key = key;
  z->parent = y;                                      // 삽입하려는 시점에는 x가 nil을, y는 nil의 parent를 가르키고 있다.(주소값을 가지고 있다)
  
  if (y == t->nil) {                                  // while문을 1번도 돌지 않았다면 y, 즉 parent는 nil일 것이다.
    t->root = z;                                      // while문을 1번도 돌지 않았다는 것은 애초에 트리가 비어있다는 뜻으로, 새로 생성한 노드 z가 rbtree의 root로 된다.
  } else if (z->key < y->key) {                       // 만약 새로 삽입할 z의 키 값이 parent의 키 값보다 작다면 parent(y)의 왼쪽에 넣어야 한다.
    y->left = z;
  } else {                                            // 만약 새로 삽입할 z의 키 값이 parent의 키 값보다 크다면 parent(y)의 오른쪽에 넣어야 한다.
    y->right = z;
  }
  
  // 새로 삽입할 노드 z는 단말 노드(리프 노드, leaf node)이기 때문에 항상 left, right는 nil이다.
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;                              // 새로 삽입할 노드의 색은 처음에 적색이다.
  rbtree_insert_fixup(t, z);                          // 특성이 위반되는 경우를 보완해주는 함수 호출
  
  // TODO: implement insert
  return t->root;                                     // 새로 생성하여 삽입한 노드의 주소를 가르키는 포인터(주소값)를 반환한다.
}  

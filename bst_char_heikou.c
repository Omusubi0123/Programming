/* 二分探索木 */

#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#define BUF_LEN 128
#define MAXCHAR 16
#define DEBUG
#define HEIKOU
//#define ENABLE_DEBUG

typedef struct t_node {
  char str[MAXCHAR];                     /* 対象のデータ(整数) */
  int bal;
  struct t_node *left, *right; /* 左右部分木へのポインタ */
} Node;

typedef Node *NodePtr;

void usage();    /* 使い方 */

int h_bal(Node *p, int h);
Node *rotate_R(Node *p);
Node *rotate_L(Node *p);
Node *rotate_control(Node *p, int dir);

Node *getNode(char key[]) ; /* 規ノードの確保 */
void bst_disp(Node *p, int indent); /* 2分探索木の表示 */
Node *bst_search(Node *p, char key[]);  /* ノードの探索 */
Node *bst_insert(Node *p, char key[]);  /* ノードの挿入 */
Node *bst_heikou(Node *p);
int bst_height(Node *p);    /* 2分探索木の高さ */
int bst_numnode(Node *p);   /* 2分探索木のノードの数 */
int bst_delete(NodePtr *pp, char key[]);    /* 2分探索機からノードを削除 */
void bst_clear(Node *p);     /* 2分探索木の全削除 */

void bst_delete_sub01(NodePtr *rp); /* 対象の節点が左の部分木がない場合 */
void bst_delete_sub02(NodePtr *rp); /* 対象の節点が右の部分木がない場合 */
void bst_delete_sub03(NodePtr *rp); /* 対象の節点が左右両方の子を持つ場合 */

int main() {
  char buf[BUF_LEN];     /* コマンド読み込み用 */
  char cmd_str[BUF_LEN]; /* コマンド文字列 */
  char cmd_key[MAXCHAR]; /* コマンド引数（整数） */
  Node *root = NULL;     /* 2分探索木の根 */

  usage();
  while (fgets(buf, BUF_LEN - 1, stdin) != NULL)
  {
    sscanf(buf, "%s%s", cmd_str, cmd_key); /* コマンド文字列の解析 */

    if (cmd_str[0] == 'Q' || cmd_str[0] == 'q') { /* 終了 */
      printf("QUIT.\n");
      bst_clear(root);
      break;
    }

    switch (cmd_str[0]) { /*コマンド文字列の先頭文字で分岐 */
    case 'S':             /* 探索: "S 探索キー" */
    case 's':
      printf("bst_search(%s) ... ", cmd_key);
      if (bst_search(root, cmd_key) != NULL)
        printf(" found.\n");
      else
        printf(" not found.\n");
      break;
    case 'I': /* 挿入: "I 探索キー" */
    case 'i':
      root = bst_insert(root, cmd_key);
      printf("bst_insert(%s) ... done.\n", cmd_key);
      break;
    case 'D': /* 削除: "D 探索キー" */
    case 'd':
      if(bst_delete(&root, cmd_key)==1)
        printf("bst_delete(%s) ... done.\n", cmd_key);
      else
        printf("bst_delete(%s) ... no in the bst.\n", cmd_key);
      break;
    case 'P': /* 表示: "P" */
    case 'p':
      printf("--- current bst ---\n");
      printf("            (RIGHT)\n");
      bst_disp(root, 10);
      printf("             (LEFT)\n");
      break;
    case 'H': /* 木の高さ: "H" */
    case 'h':
      printf("height(root) = %d\n", bst_height(root));
      break;
    case 'N':
    case 'n':
      printf("numnode(root) = %d\n", bst_numnode(root));
      break;
    default:
      printf("ERROR: unknown command ... %s\n", buf);
      break;
    }
  }

  printf("\nEND OF JOB.\n");
  return 0;
}

/* 使い方 */
void usage(void) {
  printf("---------- USAGE ----------\n");
  printf("> I 整数 ... insert\n");
  printf("> S 整数 ... search\n");
  printf("> D 整数 ... delete(under construction)\n");
  printf("> P     ... print BST\n");
  printf("> H     ... calc height of current BST\n");
  printf("> N     ... calc number of current Node\n");
  printf("> Q     ... QUIT\n");
  printf("---------------------------\n");
}

/* getNode
 *
 * 機能 : 2分探索木のノード1つ分のメモリ確保
 *
 * 引数: char key[] ... 挿入するノードに格納するデータ
 *
 * 返却値: 確保したノードの先頭アドレス / NULL(失敗)
 */
Node *getNode(char key[]) {
  Node *p = (Node *)malloc(sizeof(Node));
  if (p == NULL) {
    fprintf(stderr, "ERROR: memory allocation error.\n");
    exit(-1); /* メモリ確保できないときは強制終了 */
  }
  p->left = p->right = NULL;
  p->bal = 0;
  strcpy(p->str, key);
  return p;
}

/* bst_disp
 *
 * 機能 :
 * 引数で与えられた「2分探索木」の内容を標準出力に出力することで可視化する
 *
 * 引数: 1個
 *  Node *p : 可視化したい2分探索木（部分木）の根（ノードへのポインタ）
 *
 * 返却値: なし
 *
 */
void bst_disp(Node *p, int indent) {
  if (p == NULL)
    return;
  bst_disp(p->right, indent + 6);

  #ifdef ENABLE_DEBUG
    printf("%*s b=%d\n", indent, p->str, p->bal);
  #else
    printf("%*s\n", indent, p->str);
  #endif
  bst_disp(p->left , indent + 6);
  return;
}

#define BST_EMPTY -1
/* bst_height
 *
 * 機能 : 引数でNodeをrootと見なしたときの木の高さを計算
 *
 * 引数: 0個
 *
 * 返却値: 木の高さ(>=0) / 木が空(-1)
 *
 * 補足（動作時の返却値）
 *  空の木の場合は -1
 *  根ノード１つのみの場合は 0
 *
 */
int bst_height(Node *p) {
  /* TODO: 上記の動作となるように再帰で実装 */
  if(p == NULL)     return BST_EMPTY;  /* 空の木の場合 */
  else if(p->right == NULL && p->left == NULL)  return 0;   /* 根ノード１つのみの場合 */
  else{
    int h_left = bst_height(p->left) + 1;   /* 左部分木の高さを求める */
    int h_right = bst_height(p->right) + 1;   /* 右部分木の高さを求める */
    return (h_left > h_right)? h_left: h_right; /* 左部分木と右部分木で高さの高い方を返却 */
  }
  return 0; /* dummy */
}

/* それぞれのノードの高さを求める */
int h_bal(Node *p, int h)
{
    p->bal = h;
    if(p->right != NULL)    h_bal(p->right, h+1);
    if(p->left != NULL)     h_bal(p->left, h+1);
    return h;
}

/* 2分探索木のノードの数 */
int bst_numnode(Node *p) {
  if(p == NULL) return 0;
  return bst_numnode(p->right) + bst_numnode(p->left) + 1;
}


Node *rotate_R(Node *p)
{
    Node *l = p->left;
    int pbal_o = p->bal;
    int lbal_o = l->bal;

    p->left = l->right;
    l->right = p;

    /* pbalの更新 */
    if(lbal_o <= 0){
        p->bal = pbal_o - 1;
    }else{
        p->bal = pbal_o - lbal_o + 1;
    }
    /* rbalの更新 */
    if(p->bal <= 0){
        l->bal = lbal_o + 1;
    }else{
        l->bal = lbal_o + pbal_o + 1;
    }
    return l;
}

Node *rotate_L(Node *p)
{
    Node *r = p->right;
    int pbal_o = p->bal;
    int rbal_o = r->bal;

    p->right = r->left;
    r->left = p;

    /* pbalの更新 */
    if(rbal_o <= 0){
        p->bal = pbal_o - 1;
    }else{
        p->bal = pbal_o - rbal_o - 1;
    }
    /* rbalの更新 */
    if(p->bal >= 0){
        r->bal = rbal_o -1;
    }else{
        r->bal = rbal_o + pbal_o -1;
    }
    return r;
}

Node *rotate_control(Node *p, int dir) {
  /* dir: 左回転->+1, 右回転->-1 */
  Node *q;
  int r_bal,l_bal;
  if(dir == +1){
    q = p->right;
    r_bal = bst_height(q->right);
    l_bal = bst_height(q->left);

    if(l_bal > r_bal)
        p->right = rotate_control(q,-1);
    return rotate_L(p);
  }else if(dir == -1){
    q = p->left;
    r_bal = bst_height(q->right);
    l_bal = bst_height(q->left);

    if(l_bal < r_bal)
        p->left = rotate_control(q,+1);
    return rotate_R(p);
  }else{
    printf("ROTATE_ERROR!\n");
  }
  return p;
}

/* bst_search
 *
 * 機能 : 引数で与えられた2分探索木（部分木）を根として、指定のキーを探す
 *
 * 引数: 2個
 *  Node *p : 2分探索木の根（ノードへのポインタ）
 *  char key : 検索対象の文字列
 *
 * 返却値:
 *  == NULL : 見つからない (検索失敗)
 *  != NULL : 見つかった (検索成功) ... 見つかったデータのノードを指すポインタ
 *
 */
Node *bst_search(Node *p, char key[]) {
  int cmp=strcmp(p->str, key);
  if (p == NULL)
    return NULL; /* not found */
  if (cmp == 0)
    return p; /* found */
  else if (cmp < 0)
    return bst_search(p->left, key); /* 左部分木を探す */
  else
    return bst_search(p->right, key); /* 右部分木を探す */
}

/* 二分木の平衡状態を保つ */
Node *bst_heikou(Node *p) {
  int r_bal,l_bal;
  if(p == NULL) return p;

  r_bal = bst_height(p->right);
  l_bal = bst_height(p->left);
  if(r_bal - l_bal >=2){
    p = rotate_control(p,+1);
  }else if(l_bal - r_bal >=2){
    p = rotate_control(p,-1);
  }
  p->right = bst_heikou(p->right);
  p->left = bst_heikou(p->left);
  return p;
}

/* bst_insert
 *
 * 機能 : 引数で与えられたキーを2分探索木（部分木）に挿入
 *
 * 引数: 2個
 *  Node *p : 2分探索木の根（ノードへのポインタ）
 *  char key[] : 挿入対象の文字列
 *
 * 返却値:key
 *  != NULL : 検索対象の（部分）木の根（ノードを指すポインタ）
 *
 * 補足（動作時の返却値）
 *  一致するノードが見つからない => 新規ノードを確保しその先頭アドレスを返す
 *  一致するノードが見つかった   => 同じキーを持つノードの先頭アドレス
 *
 */
Node *bst_insert(Node *p, char key[]) {
  Node *q = p;
  if(p ==NULL){ /* 木が空の場合 */
    p = getNode(key);
  }else{
    int cmp;
    while(1)
    {
      cmp = strcmp(q->str, key);
      if(cmp == 0)  return p;  /* found (重複登録しない) */
      else if(cmp < 0){
        if(q->left==NULL){    /* 左部分木に追加 */
          q->left = getNode(key);
          break;
        }else     q=q->left;
      }else{
        if(q->right==NULL){   /* 右部分木に追加 */
          q->right = getNode(key);
          break;
        }else     q=q->right;
      }
    }
  }
  p = bst_heikou(p);
  h_bal(p,0);
  return p;
}

/* 2分探索機からノードを削除 */
int bst_delete(NodePtr *pp, char key[]) {
    NodePtr *q = pp;
    /* 削除するノードがあるか探す */
    while((*pp) != NULL)
    {
        int cmp=strcmp((*pp)->str,key);
        if(cmp==0){
            if((*pp)->left == NULL){
                bst_delete_sub01(pp);
            }else if((*pp)->right == NULL){
                bst_delete_sub02(pp);
            }else{
                /* 左部分木で最も右のノードとデータを入れ替えて削除 */
                bst_delete_sub03(pp);
            }
            *q = bst_heikou(*q);
            h_bal(*q,0);
            return 1;
        }else if(cmp<0)   pp=&((*pp)->left);
        else    pp=&((*pp)->right);
    }

    return 0;
}

/* 対象の節点が左の部分木がない場合 */
void bst_delete_sub01(NodePtr *rp) {
    NodePtr p;
    p = (*rp)->right;
    free(*rp);
    *rp=p;
    return;
}

/* 対象の節点が右の部分木がない場合 */
void bst_delete_sub02(NodePtr *rp) {
    NodePtr p;
    p = (*rp)->left;
    free(*rp);
    *rp=p;
}

/* 対象の節点が左右両方の子を持つ場合 */
void bst_delete_sub03(NodePtr *rp)
{
    NodePtr *rq;
    /* 左部分木のうち、最も右のノードを上書き */
    rq=&((*rp)->left);
    while((*rq)->right!=NULL)   rq=&((*rq)->right);
    strcpy((*rp)->str, (*rq)->str);
    /* ノード(*rq)の右部分木はないので左部分木を接続 */
    bst_delete_sub02(rq);
}

void bst_clear(Node *p) {
  if(p == NULL) return;

  bst_clear(p->right);
  bst_clear(p->left);
  free(p);
  return;
}
/* EOF (bst.c) */


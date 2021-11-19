/* �񕪒T���� */

#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#define BUF_LEN 128
#define MAXCHAR 16
#define DEBUG
#define HEIKOU
//#define ENABLE_DEBUG

typedef struct t_node {
  char str[MAXCHAR];                     /* �Ώۂ̃f�[�^(����) */
  int bal;
  struct t_node *left, *right; /* ���E�����؂ւ̃|�C���^ */
} Node;

typedef Node *NodePtr;

void usage();    /* �g���� */

int h_bal(Node *p, int h);
Node *rotate_R(Node *p);
Node *rotate_L(Node *p);
Node *rotate_control(Node *p, int dir);

Node *getNode(char key[]) ; /* �K�m�[�h�̊m�� */
void bst_disp(Node *p, int indent); /* 2���T���؂̕\�� */
Node *bst_search(Node *p, char key[]);  /* �m�[�h�̒T�� */
Node *bst_insert(Node *p, char key[]);  /* �m�[�h�̑}�� */
Node *bst_heikou(Node *p);
int bst_height(Node *p);    /* 2���T���؂̍��� */
int bst_numnode(Node *p);   /* 2���T���؂̃m�[�h�̐� */
int bst_delete(NodePtr *pp, char key[]);    /* 2���T���@����m�[�h���폜 */
void bst_clear(Node *p);     /* 2���T���؂̑S�폜 */

void bst_delete_sub01(NodePtr *rp); /* �Ώۂ̐ߓ_�����̕����؂��Ȃ��ꍇ */
void bst_delete_sub02(NodePtr *rp); /* �Ώۂ̐ߓ_���E�̕����؂��Ȃ��ꍇ */
void bst_delete_sub03(NodePtr *rp); /* �Ώۂ̐ߓ_�����E�����̎q�����ꍇ */

int main() {
  char buf[BUF_LEN];     /* �R�}���h�ǂݍ��ݗp */
  char cmd_str[BUF_LEN]; /* �R�}���h������ */
  char cmd_key[MAXCHAR]; /* �R�}���h�����i�����j */
  Node *root = NULL;     /* 2���T���؂̍� */

  usage();
  while (fgets(buf, BUF_LEN - 1, stdin) != NULL)
  {
    sscanf(buf, "%s%s", cmd_str, cmd_key); /* �R�}���h������̉�� */

    if (cmd_str[0] == 'Q' || cmd_str[0] == 'q') { /* �I�� */
      printf("QUIT.\n");
      bst_clear(root);
      break;
    }

    switch (cmd_str[0]) { /*�R�}���h������̐擪�����ŕ��� */
    case 'S':             /* �T��: "S �T���L�[" */
    case 's':
      printf("bst_search(%s) ... ", cmd_key);
      if (bst_search(root, cmd_key) != NULL)
        printf(" found.\n");
      else
        printf(" not found.\n");
      break;
    case 'I': /* �}��: "I �T���L�[" */
    case 'i':
      root = bst_insert(root, cmd_key);
      printf("bst_insert(%s) ... done.\n", cmd_key);
      break;
    case 'D': /* �폜: "D �T���L�[" */
    case 'd':
      if(bst_delete(&root, cmd_key)==1)
        printf("bst_delete(%s) ... done.\n", cmd_key);
      else
        printf("bst_delete(%s) ... no in the bst.\n", cmd_key);
      break;
    case 'P': /* �\��: "P" */
    case 'p':
      printf("--- current bst ---\n");
      printf("            (RIGHT)\n");
      bst_disp(root, 10);
      printf("             (LEFT)\n");
      break;
    case 'H': /* �؂̍���: "H" */
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

/* �g���� */
void usage(void) {
  printf("---------- USAGE ----------\n");
  printf("> I ���� ... insert\n");
  printf("> S ���� ... search\n");
  printf("> D ���� ... delete(under construction)\n");
  printf("> P     ... print BST\n");
  printf("> H     ... calc height of current BST\n");
  printf("> N     ... calc number of current Node\n");
  printf("> Q     ... QUIT\n");
  printf("---------------------------\n");
}

/* getNode
 *
 * �@�\ : 2���T���؂̃m�[�h1���̃������m��
 *
 * ����: char key[] ... �}������m�[�h�Ɋi�[����f�[�^
 *
 * �ԋp�l: �m�ۂ����m�[�h�̐擪�A�h���X / NULL(���s)
 */
Node *getNode(char key[]) {
  Node *p = (Node *)malloc(sizeof(Node));
  if (p == NULL) {
    fprintf(stderr, "ERROR: memory allocation error.\n");
    exit(-1); /* �������m�ۂł��Ȃ��Ƃ��͋����I�� */
  }
  p->left = p->right = NULL;
  p->bal = 0;
  strcpy(p->str, key);
  return p;
}

/* bst_disp
 *
 * �@�\ :
 * �����ŗ^����ꂽ�u2���T���؁v�̓��e��W���o�͂ɏo�͂��邱�Ƃŉ�������
 *
 * ����: 1��
 *  Node *p : ����������2���T���؁i�����؁j�̍��i�m�[�h�ւ̃|�C���^�j
 *
 * �ԋp�l: �Ȃ�
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
 * �@�\ : ������Node��root�ƌ��Ȃ����Ƃ��̖؂̍������v�Z
 *
 * ����: 0��
 *
 * �ԋp�l: �؂̍���(>=0) / �؂���(-1)
 *
 * �⑫�i���쎞�̕ԋp�l�j
 *  ��̖؂̏ꍇ�� -1
 *  ���m�[�h�P�݂̂̏ꍇ�� 0
 *
 */
int bst_height(Node *p) {
  /* TODO: ��L�̓���ƂȂ�悤�ɍċA�Ŏ��� */
  if(p == NULL)     return BST_EMPTY;  /* ��̖؂̏ꍇ */
  else if(p->right == NULL && p->left == NULL)  return 0;   /* ���m�[�h�P�݂̂̏ꍇ */
  else{
    int h_left = bst_height(p->left) + 1;   /* �������؂̍��������߂� */
    int h_right = bst_height(p->right) + 1;   /* �E�����؂̍��������߂� */
    return (h_left > h_right)? h_left: h_right; /* �������؂ƉE�����؂ō����̍�������ԋp */
  }
  return 0; /* dummy */
}

/* ���ꂼ��̃m�[�h�̍��������߂� */
int h_bal(Node *p, int h)
{
    p->bal = h;
    if(p->right != NULL)    h_bal(p->right, h+1);
    if(p->left != NULL)     h_bal(p->left, h+1);
    return h;
}

/* 2���T���؂̃m�[�h�̐� */
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

    /* pbal�̍X�V */
    if(lbal_o <= 0){
        p->bal = pbal_o - 1;
    }else{
        p->bal = pbal_o - lbal_o + 1;
    }
    /* rbal�̍X�V */
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

    /* pbal�̍X�V */
    if(rbal_o <= 0){
        p->bal = pbal_o - 1;
    }else{
        p->bal = pbal_o - rbal_o - 1;
    }
    /* rbal�̍X�V */
    if(p->bal >= 0){
        r->bal = rbal_o -1;
    }else{
        r->bal = rbal_o + pbal_o -1;
    }
    return r;
}

Node *rotate_control(Node *p, int dir) {
  /* dir: ����]->+1, �E��]->-1 */
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
 * �@�\ : �����ŗ^����ꂽ2���T���؁i�����؁j�����Ƃ��āA�w��̃L�[��T��
 *
 * ����: 2��
 *  Node *p : 2���T���؂̍��i�m�[�h�ւ̃|�C���^�j
 *  char key : �����Ώۂ̕�����
 *
 * �ԋp�l:
 *  == NULL : ������Ȃ� (�������s)
 *  != NULL : �������� (��������) ... ���������f�[�^�̃m�[�h���w���|�C���^
 *
 */
Node *bst_search(Node *p, char key[]) {
  int cmp=strcmp(p->str, key);
  if (p == NULL)
    return NULL; /* not found */
  if (cmp == 0)
    return p; /* found */
  else if (cmp < 0)
    return bst_search(p->left, key); /* �������؂�T�� */
  else
    return bst_search(p->right, key); /* �E�����؂�T�� */
}

/* �񕪖؂̕��t��Ԃ�ۂ� */
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
 * �@�\ : �����ŗ^����ꂽ�L�[��2���T���؁i�����؁j�ɑ}��
 *
 * ����: 2��
 *  Node *p : 2���T���؂̍��i�m�[�h�ւ̃|�C���^�j
 *  char key[] : �}���Ώۂ̕�����
 *
 * �ԋp�l:key
 *  != NULL : �����Ώۂ́i�����j�؂̍��i�m�[�h���w���|�C���^�j
 *
 * �⑫�i���쎞�̕ԋp�l�j
 *  ��v����m�[�h��������Ȃ� => �V�K�m�[�h���m�ۂ����̐擪�A�h���X��Ԃ�
 *  ��v����m�[�h����������   => �����L�[�����m�[�h�̐擪�A�h���X
 *
 */
Node *bst_insert(Node *p, char key[]) {
  Node *q = p;
  if(p ==NULL){ /* �؂���̏ꍇ */
    p = getNode(key);
  }else{
    int cmp;
    while(1)
    {
      cmp = strcmp(q->str, key);
      if(cmp == 0)  return p;  /* found (�d���o�^���Ȃ�) */
      else if(cmp < 0){
        if(q->left==NULL){    /* �������؂ɒǉ� */
          q->left = getNode(key);
          break;
        }else     q=q->left;
      }else{
        if(q->right==NULL){   /* �E�����؂ɒǉ� */
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

/* 2���T���@����m�[�h���폜 */
int bst_delete(NodePtr *pp, char key[]) {
    NodePtr *q = pp;
    /* �폜����m�[�h�����邩�T�� */
    while((*pp) != NULL)
    {
        int cmp=strcmp((*pp)->str,key);
        if(cmp==0){
            if((*pp)->left == NULL){
                bst_delete_sub01(pp);
            }else if((*pp)->right == NULL){
                bst_delete_sub02(pp);
            }else{
                /* �������؂ōł��E�̃m�[�h�ƃf�[�^�����ւ��č폜 */
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

/* �Ώۂ̐ߓ_�����̕����؂��Ȃ��ꍇ */
void bst_delete_sub01(NodePtr *rp) {
    NodePtr p;
    p = (*rp)->right;
    free(*rp);
    *rp=p;
    return;
}

/* �Ώۂ̐ߓ_���E�̕����؂��Ȃ��ꍇ */
void bst_delete_sub02(NodePtr *rp) {
    NodePtr p;
    p = (*rp)->left;
    free(*rp);
    *rp=p;
}

/* �Ώۂ̐ߓ_�����E�����̎q�����ꍇ */
void bst_delete_sub03(NodePtr *rp)
{
    NodePtr *rq;
    /* �������؂̂����A�ł��E�̃m�[�h���㏑�� */
    rq=&((*rp)->left);
    while((*rq)->right!=NULL)   rq=&((*rq)->right);
    strcpy((*rp)->str, (*rq)->str);
    /* �m�[�h(*rq)�̉E�����؂͂Ȃ��̂ō������؂�ڑ� */
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


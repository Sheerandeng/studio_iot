/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  bitree.c
 *    Description:  二叉树
 *                 
 *        Version:  1.0.0(07/08/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "07/08/19 16:11:19"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <stdlib.h>

typedef char ElemType;

typedef struct _BiTNode
{
    char    data;
    struct _BiTNode *lchild, *rchild;
}BiTNode,*BiTree;

// 创建一棵二叉树,约定用户遵照前序遍历的方式输入数据
void CreateBiTree( BiTree *T )
{
    char c;

    scanf("%c",&c);
    if( ' ' == c )
    {
        *T = NULL;
    }
    else
    {
        *T = (BiTNode *)malloc(sizeof(BiTNode));
        (*T)->data = c;
        CreateBiTree( &(*T)->lchild );
        CreateBiTree( &(*T)->rchild );
    }
}

// 访问二叉树结点的具体操作
void visit( char c, int level )
{
    printf("%c 位于第 %d 层\n",c,level);
}

// 遍历二叉树
void PreOrderTraverse( BiTree T, int level )
{
    if( T )
    {
        visit( T->data, level );
        PreOrderTraverse(T->lchild,level+1);
        PreOrderTraverse(T->rchild,level+1);
    }
}

int main()
{
    int level = 1;
    BiTree T = NULL;

    CreateBiTree(&T);
    PreOrderTraverse(T,level);

    return 0;
}

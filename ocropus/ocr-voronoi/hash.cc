/*
  $Date: 1999/10/15 12:40:27 $
  $Revision: 1.1.1.1 $
  $Author: kise $
  hash.c
  �ϥå���ؿ�

  key : 2�Ĥ�Ϣ����ʬ�Υ�٥���Ф�, id���ͤ����ؿ�
  search : id ���ͤ��ϥå���ɽ�˴�����Ͽ����Ƥ��뤫��Ĵ�٤�ؿ�
  enter : ��Ͽ����Ƥ��ʤ�id ��ϥå���ɽ����Ͽ����ؿ�
*/

#include <stdio.h>
#include "const.h"
#include "defs.h"
#include "extern.h"
#include "function.h"


namespace voronoi{
    /* �ϥå���ؿ� 1 */
    HashVal hash1(Key key)
    {
        return(key % M1);
    }

    /* �ϥå���ؿ� 2 */
    HashVal hash2(Key key)
    {
        return(key % M2);
    }

    /* ������ؿ� */
    void init_hash()
    {
        HashVal i;

        for(i=0;i<M1+M2;i++)
            hashtable[i]=NIL;
    }

    /*
     * �ϥå���ؿ����Ф��륭����Ĥ���ؿ�
     * 2�ĤΥ�٥������Ȥ�, ������Ф���id ���֤�
     */
    Key key(Label lab1, Label lab2)
    {
        unsigned long key,tmp;

        key = lab1;
        key = key << 2*BYTE;
        tmp = lab2;
        key = key | tmp;
        return key;
    }

    /* id ���ϥå���ɽ����Ͽ����Ƥ��뤫��Ĵ�٤�ؿ� */ 
    int search(Label lab1, Label lab2)
    {
        Key id;
        HashVal x;
        HashTable *p;

        id = key(lab1,lab2);
        x = hash1(id)+hash2(id);	/* �ϥå����ͤ�׻� */
        p = hashtable[x];

        while(p != NIL) {
            if((lab1 == p->lab1) && (lab2 == p->lab2)) /* ��Ͽ����Ƥ���� */
                return(p->entry);	                       /* ����entry ���ͤ��֤� */
            p = p->next;
        }
    
        return NODATA;	/* ��Ͽ����Ƥ��ʤ��Ȥ���NODATA ���ͤ��֤� */
    }

    /*
     * ��Ͽ����Ƥ��ʤ�id �Ȥ�����Ф���entry ���ͤ�
     * �ϥå���ɽ����Ͽ����ؿ�
     */ 
    void enter(Label lab1, Label lab2, unsigned int entry)
    {
        Key id;
        HashVal x;
        HashTable *p;

        id = key(lab1,lab2);
        x = hash1(id)+hash2(id);	/* �ϥå����ͤ�׻� */
    
        /* ��Ͽ���뤿����ΰ����ݤ��� */
        p = (HashTable *)myalloc(sizeof(HashTable)* 1);

        /* ���ݤ����ΰ��������, �ͤ���Ͽ���� */
        p->next = hashtable[x];
        hashtable[x] = p;
        hashtable[x]->lab1 = lab1;
        hashtable[x]->lab2 = lab2;
        hashtable[x]->entry = entry;
    }
}

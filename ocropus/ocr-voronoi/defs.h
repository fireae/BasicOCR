/*
  $Id: defs.h,v 1.1.1.1 1999/10/15 12:40:27 kise Exp $
  $Date: 1999/10/15 12:40:27 $
  $Revision: 1.1.1.1 $
  $Author: kise $
*/

#ifndef DEFS_H_INCLUDED_
#define DEFS_H_INCLUDED_
#include "const.h"

namespace voronoi{
    typedef unsigned short Coordinate;
    // Faisal Shafait's modification: changed Label type from short to int
    typedef unsigned int   Label;
    typedef unsigned int   NumPixel;
    typedef unsigned int   Key;
    typedef unsigned int   HashVal;
    typedef double         Coord;

    /* �٥��ȥ��ɽ����¤��
       the structure for a vector */
    typedef struct{
        int x;
        int y;
    } Vector;

    /* ������ɽ����¤��
       the structure for a binary image */ 
    typedef struct{
        char *image;
        Coordinate imax, jmax; /* width and height of the image */
    } ImageData;

    /* ���ǤΥ�٥�Ⱥ�ɸ��ɽ����¤��
       the structure for a pixel */
    typedef struct{
        Label label;     /* label */
        Coordinate xax,yax; /* coordinates */
    } BlackPixel;

    /* ��Ϣ����ʬ�νſ���ɸ��, �����Ϥ����νĲ���Ĺ����ɽ����¤�� */
    /*
      typedef struct{
      unsigned short x,y;
      unsigned short dx,dy;
      unsigned int bpn;
      } Component;
    */

    /* ���ܤ���Ϣ����ʬ�δط���ɽ����¤��
       the structure for a neighboring relation
       between connected components(CC's)

       lab2
       -------
       |  x  |
       -------
       --- /
       | |/ angle
       |x|-----
       | |
       ---
       lab1
    */
    typedef struct{
        float dist;       /* min. distance between CC's */
        float angle;      /* angle between CC's */
        Label lab1, lab2; /* labels of CC's */
    } Neighborhood;

    /* �ܥ�Υ��դλ�����������, �ɤ�����Ϣ����ʬ�֤Ǥ��뤫��ɽ����¤��
       the structure for a Voronoi edge */
    typedef struct{
        int	sp,ep;
        Coordinate xs,xe,ys,ye;  /* + (xs,ys)
                                    \
                                    \  Voronoi edge
                                    \
                                    + (xe,ye)
                                 */
        Label lab1,lab2;  /* this Voronoi edge is between
                             CC of a label "lab1" and that of lab2 */
        unsigned short yn;
    } LineSegment;

    /* �ܥ�Υ��դ�ü��
       the structure for a Voronoi point */
    typedef struct node{
        int line;
        struct node *next;
    } EndPoint;

    /* ����Ϣ����ʬ�Υ�٥���Ф���ϥå���ɽ
       the structure for a hash table for
       representing labels of CC's */
    typedef struct hash {
        Label lab1;
        Label lab2;
        unsigned int entry;
        struct hash *next;
    } HashTable;
    /*
      typedef struct hash {
      unsigned long id;
      unsigned int entry;
      struct hash *next;
      } HashTable;
    */

    /* �����ɽ����¤��
       the structure for a rectangle */
    typedef struct{
        Coordinate is,ie,js,je;
    } Rect;

    struct Freenode {
        struct Freenode	*nextfree;
    };

    struct Freelist {
        struct Freenode	*head;
        // Faisal Shafait's modification to fix memory leak
#ifdef h_iupr_
        colib::narray<void *> allocated_chunks;
#endif
        // End of Modification
        int		nodesize;
    };

    struct Point {
        float		x,y;
    };

    /* structure used both for sites and for vertices */
    struct Site {
        struct Point	coord;
        int		sitenbr;
        int		refcnt;
        unsigned int	label;
    };

    struct Edge	{
        float		a,b,c;
        struct	Site 	*ep[2];
        struct	Site	*reg[2];
        int		edgenbr;
        unsigned int	lab1,lab2;
        float		dist;
    };

    struct Halfedge {
        struct Halfedge	*ELleft, *ELright;
        struct Edge	*ELedge;
        int		ELrefcnt;
        char		ELpm;
        struct Site	*vertex;
        float		ystar;
        struct Halfedge	*PQnext;
    };
}
#endif /* DEFS_H_INCLUDED_ */

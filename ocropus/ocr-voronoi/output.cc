/*
  $Date: 1999/10/15 12:40:27 $
  $Revision: 1.1.1.1 $
  $Author: kise $
  output.c
*/

#include <stdio.h>
#include <math.h>
#include "const.h"
#include "defs.h"
#include "extern.h"
#include "function.h"


namespace voronoi{
    /* float pxmin, pxmax, pymin, pymax, cradius; */

    /*
     * (x,y)�κ�ɸ���ȳ��Ǥ����, ����κ�ɸ���Ѵ�����ؿ�
     * �������ܥ�Υ���(x,y)���ֹ��FRAME(-1)�ˤ���.
     */
    void in_frame(float *x, float *y, float d,
                  struct Edge *e, int lr,
                  Coordinate max_x, Coordinate max_y)
    {
        /* x �κ�ɸ��0 �ʲ��ξ�� */
        if(*x < 0){
            *y += (-*x)*d;
            *x = 0;
            e->ep[lr]->sitenbr = FRAME;
            if(*y < 0){
                *x += (-*y)/d;
                *y = 0;
            }
            else if(*y > max_y){
                *x += ((float)max_y - *y)/d;
                *y = (float)max_y;
            }
        }
        /* x �κ�ɸ��max_x �ʾ�ξ�� */
        else if(*x > max_x){
            *y += ((float)max_x - *x)*d;
            *x = (float)max_x;
            e->ep[lr]->sitenbr = FRAME;
            if(*y < 0){
                *x += (-*y)/d;
                *y = 0;
            }
            else if(*y > max_y){
                *x += ((float)max_y - *y)/d;
                *y = (float)max_y;
            }
        }
        /* y �κ�ɸ��0 �ʲ��ξ�� */
        else if(*y < 0){
            *x += (-*y)/d;
            *y = 0;
            e->ep[lr]->sitenbr = FRAME;
        }
        /* y �κ�ɸ��max_y �ʾ�ξ�� */
        else if(*y > max_y){
            *x += ((float)max_y - *y)/d;
            *y = (float)max_y;
            e->ep[lr]->sitenbr = FRAME;
        }
    }

    /* ̵�±����Ǥ������������κ�ɸ���Ѵ�����ؿ� */
    void s_in_frame(float *xsf, float *xef, float *ysf, float *yef,
                    struct Edge *e, Coordinate max_y)
    {
        float d;
    
        if((e->b) == 0) {	/* ��������ξ�� */
            *xsf = *xef;
            *ysf = (float)max_y;
        }
        else {
            d = -(e->a)/(e->b); /* ���� */
	    
            if(d == 0){	/* ������0 �ξ�� */
                *xsf = 0;
                *ysf = *yef;
            }
            else if(d > 0){	/* ���������ξ�� */
                if(*yef > *xef*d){
                    *xsf = 0;
                    *ysf = *yef - (*xef*d);
                }
                else {
                    *xsf = *xef - (*yef/d);
                    *ysf = 0;
                }
            }
            else {		/* ��������ξ�� */
                if(((float)max_y - *yef) > ((-*xef) * d)){
                    *xsf = 0;
                    *ysf = *yef + ((-*xef) * d);
                }
                else {
                    *xsf = *xef - (-((float)max_y - *yef)) / d;
                    *ysf = (float)max_y;
                }
            }
        }
    }

    /* ̵�±����Ǥ��뽪��������κ�ɸ���Ѵ�����ؿ� */
    void e_in_frame(float *xsf, float *xef, float *ysf, float *yef,
                    struct Edge *e, Coordinate max_x, Coordinate max_y)
    {
        float d;

        if((e->b) ==0) {	/* ��������ξ�� */
            *xef = *xsf;
            *yef = 0;
        }
        else {
            d = -(e->a)/(e->b); /* ���� */

            if(d == 0){	/* ������0 �ξ�� */
                *xef = (float)max_x;
                *yef = *ysf;
            }
            else if(d > 0){	/* ���������ξ�� */
                if(((float)max_y - *ysf) > (((float)max_x - *xsf) * d)){
                    *xef = (float)max_x;
                    *yef = *ysf + ((float)max_x - *xsf) * d;
                }
                else {
                    *xef = *xsf + ((float)max_y - *ysf) / d;
                    *yef = (float)max_y;
                }
            }
            else {		/* ��������ξ�� */
                if(*ysf > (- ((float)max_x - *xsf) * d)){
                    *xef = (float)max_x;
                    *yef = *ysf - (- ((float)max_x - *xsf) * d);
                }
                else {
                    *xef = *xsf + (-*ysf) / d;
                    *yef =0;
                }
            }
        }
    }

    /*
     * ����, �����κ�ɸ��̵�±���, �ޤ���, �ȳ��ξ��,
     * ��ɸ������, ���Υܥ�Υ������ֹ��FRAME(-1) �ˤ���ؿ�
     */
    void frameout(float *xsf, float *xef, float *ysf, float *yef,
                  int *sp, int *ep, struct Edge *e,
                  Coordinate max_x, Coordinate max_y)
    {
        float d;

        /* ����, �����Ȥ��̵�±����Ǥʤ���� */
        if((e->ep[LE] != (struct Site *)NULL) &&
           (e->ep[RE] != (struct Site *)NULL)){
            *xsf = e->ep[LE]->coord.x; /* ������x ��ɸ */
            *xef = e->ep[RE]->coord.x; /* ������x ��ɸ */
            *ysf = e->ep[LE]->coord.y; /* ������y ��ɸ */
            *yef = e->ep[RE]->coord.y; /* ������y ��ɸ */

            /* �������� �ΤȤ� */
            if(*xsf == *xef) {
                if(*ysf < 0) {
                    *ysf =0;
                    e->ep[LE]->sitenbr = FRAME;
                }
                else if(*ysf > max_y) {
                    *ysf = (float)max_y;
                    e->ep[LE]->sitenbr = FRAME;
                }
                if(*yef < 0) {
                    *yef = 0;
                    e->ep[RE]->sitenbr = FRAME;
                }
                else if(*yef > max_y) {
                    *yef = (float)max_y;
                    e->ep[RE]->sitenbr = FRAME;
                }
            }

            /* ������ͭ�¤ΤȤ� */
            else {
                d = (*yef - *ysf)/(*xef - *xsf); /* ���� */

                /* �������������ȳ��ξ�� */
                in_frame(xsf,ysf,d,e,LE,max_x,max_y);
                /* �������������ȳ��ξ�� */	    
                in_frame(xef,yef,d,e,RE,max_x,max_y);
            }
	
            *sp = e->ep[LE]->sitenbr; /* �������ֹ������ */
            *ep = e->ep[RE]->sitenbr; /* �������ֹ������ */
	
        }
    
        /* ������̵�±����ξ�� */
        else if((e->ep[LE] == (struct Site *)NULL)) {
            *xef = e->ep[RE]->coord.x; /* ������x ��ɸ */
            *yef = e->ep[RE]->coord.y; /* ������y ��ɸ */
            *sp = FRAME;	/* �������ֹ�FRAME ������ */
            *ep = e->ep[RE]->sitenbr; /* �������ֹ������ */

            /* ����������ˤ��� */
            s_in_frame(xsf,xef,ysf,yef,e,max_y);
        }
        
        /* ������̵�±����ξ�� */
        else if((e->ep[RE] == (struct Site *)NULL)){
            *xsf = e->ep[LE]->coord.x; /* ������x ��ɸ */
            *ysf = e->ep[LE]->coord.y; /* ������y ��ɸ */
            *sp = e->ep[LE]->sitenbr; /* �������ֹ������ */
            *ep = FRAME;	/* �������ֹ�FRAME ������ */

            /* ����������ˤ��� */
            e_in_frame(xsf,xef,ysf,yef,e,max_x,max_y);
        }
    }

    /*
     * Ϣ����ʬ�֤Υܥ�Υ��դΤߤ�lineseg �˳�Ǽ��,
     * Ϣ����ʬ�֤δط�neighbor ��Ĥ���ؿ�.
     */
    void out_ep2(struct Edge *e, struct Site *v,
                 Coordinate imax, Coordinate jmax)
    {
        int i,sp,ep;
        float xsf,xef,ysf,yef;
        //  float si,sj,ei,ej;
        Coordinate max_x=imax-1;
        Coordinate max_y=jmax-1;

        static unsigned int current_neighbor_size = INITNEIGHBOR;
        static unsigned int current_lineseg_size  = INITLINE;

        /* double i1,j1,i2,j2; */



        /* ��٥뤬Ʊ�����Ͻ��Ϥ��ʤ� */
        if(output_pvor == NO && e->lab1 == e->lab2) {
            point_edge++;
            return;
        }
        else {
            /*
             * �ܥ�Υ�����̵�±���, �ޤ���, �ȳ����ɤ���Ƚ�ꤷ,
             * �⤷, �����ʤ�����κ�ɸ�˽���
             */
            frameout(&xsf,&xef,&ysf,&yef,&sp,&ep,e,imax-1,jmax-1);
	
            /* ��ν����ˤ�äƤ����, �����ΰ������ȳ��ξ����Ϥ��ʤ� */
            if((xsf < 0.0) || (xsf > (float)max_x) ||
               (xef < 0.0) || (xef > (float)max_x) ||
               (ysf < 0.0) || (ysf > (float)max_y) ||
               (yef < 0.0) || (yef > (float)max_y))
                return;
        }

        // ���ꥸ�ʥ�Ȱ㤦�Ȥ���
        // ���٥����ȥ����Ȥ��Ƥߤ�
        //
        //  if((e->ep[LE] != (struct Site *)NULL) &&
        //     (e->ep[RE] != (struct Site *)NULL)){
        //    /* �ܥ�Υ��դλ����������Ȥ��̵�±����Ǥʤ���� */
        //		
        //    si = e->ep[LE]->coord.x; /* ������x ��ɸ */
        //    ei = e->ep[RE]->coord.x; /* ������x ��ɸ */
        //    sj = e->ep[LE]->coord.y; /* ������y ��ɸ */
        //    ej = e->ep[RE]->coord.y; /* ������y ��ɸ */
        //		
        //    if(!((si < (float)imax)&&(ei < (float)imax)&&
        //	 (sj < (float)jmax)&&(ej < (float)jmax)
        //	 &&(si > 0)&&(ei > 0)&&(sj > 0)&&(ej > 0))){
        //      /* �⤷�����������Ȥ�˲������ˤ��ä��� */
        //      return;
        //      /* neighbor �Υ��åȤ�Ԥ�ʤ� */
        //    }
        //  }
        //  else{
        //    /* �ܥ�Υ��դλ����������Τɤ��餫��̵�±����Ǥ����� */
        //    return;
        //    /* neighbor �Υ��åȤ�Ԥ�ʤ� */
        //  }
	
        /*
         * Ϣ����ʬ�����ܴط�neighbor �˴�����Ͽ���Ƥ����,
         * ��Ͽ����Ƥ����Υ��, ����Υܥ�Υ��դ������������֤�
         * ��Υ���礭������Ӥ�,Ϣ����ʬ�֤ε�Υ����������
         */

        i = search(e->lab1,e->lab2); /* �ϥå���ɽ����Ͽ����Ƥ��뤫Ĵ�٤� */

        /* ̤��Ͽ�ξ�� */

        if(i == NODATA){
            enter(e->lab1,e->lab2,NEIGHnbr); /* �ϥå���ɽ����Ͽ */
            /*
              i1 = (double)(component[e->lab1].xc);
              j1 = (double)(component[e->lab1].yc);
              i2 = (double)(component[e->lab2].xc);
              j2 = (double)(component[e->lab2].yc);
            */

            /* �����֤ε�Υ������ */
            neighbor[NEIGHnbr].dist = e->dist; /* Ϣ����ʬ�֤κǾ���Υ */
            neighbor[NEIGHnbr].lab1 = e->lab1;
            neighbor[NEIGHnbr].lab2 = e->lab2;
	
            /* �ſ��֤γ��٤����� */
            /*	if(i1 == i2) {
                neighbor[NEIGHnbr].angle = -RIGHTANGLE;
                }
                else {
                angle = atan2((j2-j1),(i2-i1))*2*RIGHTANGLE/M_PI;
			
                if(angle > RIGHTANGLE){
                neighbor[NEIGHnbr].angle = (float)(angle-2*RIGHTANGLE);
                }
                else if(angle <= -RIGHTANGLE){
                neighbor[NEIGHnbr].angle = (float)(angle+2*RIGHTANGLE);
                }
                else {
                neighbor[NEIGHnbr].angle = (float)angle;
                }
                }
            */
            /*	neighbor[NEIGHnbr].dist = 
                (float)sqrt((component[e->lab1].xc - component[e->lab2].xc) *
                (component[e->lab1].xc - component[e->lab2].xc) +
                (component[e->lab1].yc - component[e->lab2].yc) *
                (component[e->lab1].yc - component[e->lab2].yc));*/
            /* Ϣ����ʬ�νſ��֤ε�Υ */
            /*	dx = (double)(component[e->lab2].xc - component[e->lab1].xc);
                dy = (double)(component[e->lab2].yc - component[e->lab1].yc);
                neighbor[NEIGHnbr].angle = (float)atan2(dy,dx);*/
	
            NEIGHnbr++;
            if(NEIGHnbr >= current_neighbor_size) {
                neighbor=(Neighborhood *)myrealloc(neighbor,
                                                   current_neighbor_size,
                                                   INCNEIGHBOR,
                                                   sizeof(Neighborhood));
                current_neighbor_size+=INCNEIGHBOR;
            }
        }

        /* ��Ͽ�Ѥߤξ�� */
        else {
            if(neighbor[i].dist > e->dist) /* ��Υ����Ӥ���û������Ϣ�� */
                neighbor[i].dist = e->dist; /* ��ʬ�֤ε�Υ�Ȥ��� */
        }

        if(sp > SiteMax) SiteMax = sp;
        if(ep > SiteMax) SiteMax = ep;

        /* �ܥ�Υ��դξ�����Ǽ���� */
        lineseg[LINEnbr].sp = sp;
        lineseg[LINEnbr].ep = ep;
        lineseg[LINEnbr].xs = (unsigned int)(xsf+0.5);
        lineseg[LINEnbr].xe = (unsigned int)(xef+0.5);
        lineseg[LINEnbr].ys = (unsigned int)(ysf+0.5);
        lineseg[LINEnbr].ye = (unsigned int)(yef+0.5);
        lineseg[LINEnbr].lab1 = e->lab1;
        lineseg[LINEnbr].lab2 = e->lab2;
        lineseg[LINEnbr].yn = OUTPUT;
        LINEnbr++;
        point_edge++;
        if(LINEnbr >= current_lineseg_size) {
            lineseg=(LineSegment *)myrealloc(lineseg,
                                             current_lineseg_size,
                                             INCLINE,
                                             sizeof(LineSegment));
            current_lineseg_size+=INCLINE;
        }
    }
}

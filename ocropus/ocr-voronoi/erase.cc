/*
  $Date: 1999/10/18 08:59:27 $
  $Revion: $
  $Author: kise $
  erase.c
  �ܥ�Υ��ս���ץ����

  1�ĤΥܥ�Υ��դ��б�����Ϣ����ʬ�֤ε�Υ��,
  ���Ͱʲ��ξ��, ���Υܥ�Υ��դ����.
  ���η��, �Ǥ���ñ��ü������ĥܥ�Υ��դ����

  endp	: ��������Υ���ȥ�ϥܥ�Υ��դ�ü�����ֹ�
  ��Ƭ����(line)��, ���Υܥ�Υ�����ü���Ȥ��ƻ���
  ��ʬ(�ܥ�Υ���)�ο�
  ���θ�˳��ݤ��줿�ΰ����(line)��, ���Υܥ�Υ�����
  ü���Ȥ��ƻ�����ʬ(�ܥ�Υ���)���ֹ�
  lineseg : �ܥ�Υ��դ�ü��(�ܥ�Υ���)���ֹ��,
  �ܥ�Υ��դλ���, �����κ�ɸ,
  ���ϵ��ľ���򵭲�
*/

#include <stdio.h>
#include <math.h>
#include "const.h"
#include "defs.h"
#include "extern.h"
#include "function.h"

namespace voronoi{
    extern int              *area;

    float			Td2=0.0;	/* Ƚ�̼��η��� */
    unsigned int		Td1=0;		/* ��Υ������ */

    unsigned int            Dmax;

    int start_pos (int pos)
    {
        int cpos = pos - smwind;
        if (cpos < 0) {
            return 0;
        }
        else {
            return cpos;
        }
    }

    int end_pos (int pos)
    {
        int cpos = pos + smwind;
        if (cpos >= Dmax){
            return Dmax-1;
        }
        else {
            return cpos;
        }
    }

    unsigned int Dh_ave(unsigned int *Dh, int pos)
    {
        int i;
        unsigned int ave=0;

        int start = start_pos(pos);
        int end = end_pos(pos);

        for(i=start;i<=end;i++){
            ave=ave+Dh[i];
        }
        return ( (unsigned int) (ave / (end - start + 1)) );
    }

    /*
      ��Υ, �����ǿ�����(��), ʿ�ѹ����Ĺ�κ��Υҥ��ȥ�������ؿ�
    */

    void hist()
    {
        int i,j;
        unsigned int *Dh, *Dh_ref, max1, max2;
        float freq,freq1,freq2;

        Dmax = 0;
    
        /* ��Υ, �����Ǥ���(��), ʿ�ѹ����Ĺ�κ��κ����ͤ���� */
        for(i=0;i<NEIGHnbr;i++) {
            if(Dmax < neighbor[i].dist)
                Dmax = (unsigned int)(neighbor[i].dist+0.5);
        }
        Dmax++;
    
        /* ��Υ�Υҥ��ȥ������������ΰ���� */
        Dh = (unsigned int *)myalloc(sizeof(unsigned int)* Dmax);
        Dh_ref = (unsigned int *)myalloc(sizeof(unsigned int)* Dmax);

        /* ��������� */
        for(i=0;i<Dmax;i++) {
            init_u_int(&Dh[i]);
            init_u_int(&Dh_ref[i]);
        }
    
        /* �ҥ��ȥ������� */
        for(i=0;i<NEIGHnbr;i++) {
            Dh[(int)(neighbor[i].dist+0.5)]++;
        }

        /* �ҥ��ȥ����Υ��ࡼ���� */
        for(i=0;i<Dmax;i++){
            Dh_ref[i]=Dh[i];
        }
        for(i=0;i<Dmax;i++){
            Dh[i]=Dh_ave(Dh_ref,i);
        }

        /* �ҥ��ȥ�����Ĵ�٤� */

        /* ��Υ�˴ؤ��������Td2 ���ͤ���� */
        max1 = max2  = 0;
        for(i=1;i<Dmax-1;i++) {
	
            /* i ���������ξ�� */
            if(Dh[i-1] < Dh[i] && Dh[i] > Dh[i+1]){
                if(Dh[max1] < Dh[i]) {
                    max2 = max1;
                    max1 = i;
                }
                else if(Dh[max2] < Dh[i])
                    max2 = i;
            }
            else if(Dh[i-1] == Dh[i] && Dh[i] > Dh[i+1]) {
                for(j=i-2;j>=0;j--) {
                    if(Dh[j] < Dh[i]) {
                        if(Dh[max1] < Dh[i]) {
                            max2 = max1;
                            max1 = i;
                        }
                        else if(Dh[max2] < Dh[i])
                            max2 = i;

                        break;
                    }
                    else if(Dh[j] > Dh[i])
                        break;
                }
            }
        }
        if(max1 > max2) {
            i = max2;
            max2 = max1;
            max1 = i;
        }
    
        /*
          (i,Dh[i])��(i+1,Dh[i+1])�δ֤������䴰
        */
        freq=(float)Dh[max2]*freq_rate;
        for(i=max2 ; i<Dmax-1 ; i++) {
            freq1=(float)Dh[i];
            freq2=(float)Dh[i+1];
            if(freq1 >= freq && freq >= freq2){
                if(freq1 != freq2){
                    Td2=(freq1*(float)(i+1)-freq2*(float)i-freq)/(freq1-freq2);
                }
                else{
                    for(j=i+1;j<Dmax;j++){
                        if(Dh[j]!=freq){
                            Td2=(float)(i+j-1)/2.0;
                            /* ʿ�Ԥξ������� */
                            break;
                        }
                    }
                }
                break;
            }
        }

        Td1 = max1;

        //fprintf(stderr,
        //        "\n\tdist\tmax1 %d  max2 %d : Td1 %d  Td2 %.1f  Ta %d\n",
        //        max1,max2,Td1,Td2,Ta);

        if(Td2 == 0.0) {
            fprintf(stderr,"����Td2���ͤ�0�Ǥ�\n");
            exit(1);
        }

        /* �ΰ���� */
        free(Dh);
        free(Dh_ref);
    }

    /*
      2�Ĥ�Ϣ����ʬ�֤ε�Υ, �����ǿ��κ�, (ʿ�ѹ����Ĺ�κ�)����,
      �ܥ�Υ��դ��Ȥꤢ�������ϲ�ǽ��Ƚ�̤���ؿ�
    */   
    int distinction(Label lab1, Label lab2, int j)
    {
        float dist,dxy,xy1,xy2,n;
            
        /* Ϣ����ʬ�֤ε�Υ */
        dist = neighbor[j].dist;
    
        if(dist <= Td1) return(NO_OUTPUT);	/* ���Ϥ��ʤ�(�����) */

        /* 2�Ĥ�Ϣ����ʬ�ι����ǿ��ο� */
        xy1 = (float)area[lab1];
        xy2 = (float)area[lab2];
        if(xy1 > xy2)
            dxy = xy1 / xy2;
        else
            dxy = xy2 / xy1;
        n = dist / Td2 + dxy / (float)Ta;
        if(n > 1){
            return(OUTPUT);		/* ����(����ʤ�) */
        }
        else{
            return(NO_OUTPUT);	/* ���Ϥ��ʤ�(�����) */
        }
    }

    /* ñ��ü������ĥܥ�Υ��դ�����ؿ� */
    void erase_endp(int j)
    {
        EndPoint *point;

        while(j != FRAME) {
            endp[j].line--;
            /* �ܥ�Υ��դο���1 �ξ�� */
            if(endp[j].line == 1) {
                point = endp[j].next;
                /* �ܥ�Υ��դο���0 �ˤ��� */
                endp[j].line = 0;
                while(1) {
                    /* �ޤ����Ϥ���ȤʤäƤ���ܥ�Υ��դΤ⤦������ü�� */
                    if(lineseg[point->line].yn == OUTPUT) {
                        if(j == lineseg[point->line].sp){
                            j = lineseg[point->line].ep;
                        }
                        else{
                            j = lineseg[point->line].sp;
                        }
                        /* ���Ϥ��ʤ�����  */
                        lineseg[point->line].yn = NO_OUTPUT;
                        /* ������j ���������н��� */
                        break;
                    }
                    /* �ܥ�Υ��դΥꥹ�Ȥ�����������ä���� */
                    else if(point->next == NULL){
                        return;
                    }
                    /* ����ƤӽФ� */
                    else point = point->next;
                }
                continue;
            }
            else return;
        }
    }

    void erase_aux()
    {
        int i,j;
        EndPoint *point;

        /* endp ������ */
        for(i=0;i<SiteMax;i++){
            init_int(&endp[i].line);
            endp[i].next = NULL;
        }

        /*
         * Ϣ����ʬ�֤ε�Υ��, ����(THRESHOLD)�ʾ��
         * �ܥ�Υ��դ�Ĵ�٤�
         */
        for(i=0;i<LINEnbr;i++) {
            j = search(lineseg[i].lab1,lineseg[i].lab2);

            /* �ܥ�Υ��դ�Ƚ�̼�����, �Ȥꤢ�������ϲĤʤ� */
            if(distinction(lineseg[i].lab1,lineseg[i].lab2,j)
               == OUTPUT) {

                /* �����������α�Ǥʤ���� */
                if(lineseg[i].sp != FRAME) {
                    point = (EndPoint *)myalloc(sizeof(EndPoint)* 1);
	    
                    /* ���ݤ����ΰ��endp[] �ˤĤʤ��� */
                    point->next = endp[lineseg[i].sp].next;
                    endp[lineseg[i].sp].next = point;
                    point->line = i;
                    endp[lineseg[i].sp].line ++;
                }

                /* �����������α�Ǥʤ���� */
                if(lineseg[i].ep != FRAME) {
                    point = (EndPoint *)myalloc(sizeof(EndPoint)* 1);
	   
                    /* ���ݤ����ΰ��endp[] �ˤĤʤ��� */		
                    point->next = endp[lineseg[i].ep].next;
                    endp[lineseg[i].ep].next = point;
                    point->line = i;
                    endp[lineseg[i].ep].line ++;
                }

                lineseg[i].yn = OUTPUT;	/* �Ȥꤢ�������Ϥ��Ƥ褤�� */
					/* ���� */
            }

            /* ���Ϥ��ʤ� */
            else {
                lineseg[i].yn = NO_OUTPUT;
            }
        }

        /* ü������ */
        for(i=0;i<SiteMax;i++) {
            /* 1�ĤΥܥ�Υ��դˤ���°���ʤ�ü���ΤȤ� */
            if(endp[i].line == 1) {
                point = endp[i].next;

                endp[i].line = 0;	/* �ܥ�Υ��դο���0 �ˤ��� */

                lineseg[point->line].yn = NO_OUTPUT; /* ���Ϥ��ʤ����� */
	    
                /* �ܥ�Υ��դΤ⤦������ü������� */
                if(i == lineseg[point->line].sp){
                    j = lineseg[point->line].ep;
                }
                else if(i == lineseg[point->line].ep) {
                    j = lineseg[point->line].sp;
                }
                else {
                    fprintf(stderr,"erase() error(1) !!\n");
                    exit(1);
                }
	    	    
                erase_endp(j);	/* ��j ������Ǥ��뤫Ĵ��, �����н�
				   ��� */
            }
        }
    }

    /* �ܥ�Υ��ս���ؿ� */
    void erase()
    {
        /* �ҥ��ȥ�������, Ƚ�̼������ͷ׻� */
        hist();
        erase_aux();
    }

    /* unsigned int �������� */
    void init_u_int(unsigned int *data)
    {
        *data = 0;
    }

    /* int �������� */
    void init_int(int *data)
    {
        *data = 0;
    }
}

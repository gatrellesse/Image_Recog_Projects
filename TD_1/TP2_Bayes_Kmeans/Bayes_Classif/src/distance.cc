#include "Image.hh"
#include "distance.hh"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

void Euclide(Image<int>& p,int moy_x,int moy_y) {
  int index=0;
  int* PIX=p.PI();
  int L=p.PL();
  int W=p.PW();
  int i,j;
  int max = 0;

  for (j=0;j<L;j++) 
    for (i=0;i<W;i++) {
      PIX[index] = (int)sqrt(float((i-moy_x)*(i-moy_x)+(j-moy_y)*(j-moy_y)));
      if (max<PIX[index]) max=PIX[index];
      index++;
    }
  index=0;
  for (j=0;j<L;j++) 
    for (i=0;i<W;i++) {
      PIX[index]=PIX[index]*255/max;
      if ((PIX[index]%50)<2) PIX[index]=501;
      index++;
    }
}

void Euclide2(Image<int>& p,int moy1_x,int moy1_y,int moy2_x,int moy2_y) {
  int index=0;
  Image<int> q(p);
  Image<int> l(p);
  int* PIX=p.PI();
  int* QPIX=q.PI();
  int* LPIX=l.PI();
  int L=p.PL();
  int W=p.PW();
  int i,j,val;
  int max = 0;

  for (j=0;j<L;j++) 
    for (i=0;i<W;i++) {
      if ((moy1_x > 0)&&(moy1_y > 0)) 
        PIX[index] = (int)sqrt(float((i-moy1_x)*(i-moy1_x)+(j-moy1_y)*(j-moy1_y)));
      else PIX[index] = L+W;
      if ((moy2_x > 0)&&(moy2_y > 0)) 
        QPIX[index] = (int)sqrt(float((i-moy2_x)*(i-moy2_x)+(j-moy2_y)*(j-moy2_y)));
      else QPIX[index] = L+W;
      if (PIX[index]<QPIX[index]) {
         val = PIX[index];
         LPIX[index] = 0;
       } else {
         val = QPIX[index];
         LPIX[index] = 1;
      }	
      if (max < val) max = val;
      index++;
    }
  index=0;
  for (j=0;j<L;j++) 
    for (i=0;i<W;i++) {
      if (PIX[index]<QPIX[index]) {
        PIX[index] = (int)(PIX[index]*255.0)/((float)(max));
        PIX[index] *= 65536;//niveaux de bleus
        if (((PIX[index]%50)<2)&&(PIX[index]>10)) PIX[index]=65536*255;//(0,0,255)
      } else {
        PIX[index]=(int)(QPIX[index]*255.0)/((float)(max));//niveaux de rouge
        if (((PIX[index]%50)<2)&&(PIX[index]>10)) PIX[index]=255;//(255,0,0)
      }
      if (LPIX[index] && !(l.X(i-1,j)&&l.X(i+1,j)&&l.X(i,j-1)&&l.X(i,j+1)))
          PIX[index] = 255 + 255*65536;//(255,0,255)
      index++;
    }
}


void Mahalanobis(Image<int>& p,int moy_x,int moy_y,float a11,float a21,float a22) {
  int index=0;
  float *DIST;
  int* PIX=p.PI();
  int L=p.PL();
  int W=p.PW();
  int i,j;
  float max = 0;

  DIST = (float*)calloc(W*L,sizeof(float));
  for (j=0;j<L;j++) 
    for (i=0;i<W;i++) {
      DIST[index] = sqrt(a11*(i-moy_x)*(i-moy_x)+
			 2*a21*(i-moy_x)*(j-moy_y)+
			 a22*(j-moy_y)*(j-moy_y));
      if (max<DIST[index]) max=DIST[index];
      index++;
    }
  index=0;
  for (j=0;j<L;j++) 
    for (i=0;i<W;i++) {
      PIX[index]=(int)((DIST[index]*255)/max);
      if ((PIX[index]%50)<2) PIX[index]=501;
      index++;
    }
  free(DIST);
}

void Bayes_Cloud(Image<int>& p,int moy1_x,int moy1_y,float det1,float a11,float a21,float a22,
 			int moy2_x,int moy2_y,float det2,float b11,float b21,float b22,float proba_c1) {
  int index=0;
  Image<int> q(p);
  Image<int> l(p);
  int* PIX=p.PI();
  int* LPIX=l.PI();
  float *DIST_1;
  float *DIST_2;
  int L=p.PL();
  int W=p.PW();
  int i,j;
  float val1,val2,valdist;
  float max = 0.0;
  float PI = 3.1415926;

  DIST_1 = (float*)calloc(W*L,sizeof(float));
  DIST_2 = (float*)calloc(W*L,sizeof(float));
  for (j=0;j<L;j++) 
    for (i=0;i<W;i++) {
      if ((moy1_x > 0)&&(moy1_y > 0)) {
         val1 = proba_c1*exp(-0.5*(a11*(i-moy1_x)*(i-moy1_x)+
			   2*a21*(i-moy1_x)*(j-moy1_y)+
			   a22*(j-moy1_y)*(j-moy1_y)))/(2*PI*sqrt(det1));
         DIST_1[index] = sqrt(a11*(i-moy1_x)*(i-moy1_x)+
			   2*a21*(i-moy1_x)*(j-moy1_y)+
			   a22*(j-moy1_y)*(j-moy1_y));
       } else DIST_1[index] = 0.0;
      if ((moy2_x > 0)&&(moy2_y > 0)) {
        val2 = (1-proba_c1)*exp(-0.5*(b11*(i-moy2_x)*(i-moy2_x)+
			   2*b21*(i-moy2_x)*(j-moy2_y)+
			   b22*(j-moy2_y)*(j-moy2_y)))/(2*PI*sqrt(det2));
        DIST_2[index] = sqrt(b11*(i-moy2_x)*(i-moy2_x)+
			   2*b21*(i-moy2_x)*(j-moy2_y)+
			   b22*(j-moy2_y)*(j-moy2_y));
      } else DIST_2[index] = 0.0;
      if (val1 > val2) {
         valdist = DIST_1[index];
         LPIX[index] = 0;
       } else {
         valdist = DIST_2[index];
         LPIX[index] = 1;
      }	
      if (max < valdist) max = valdist;
      index++;
    }
  // Affichage en couleur selon la classe
  index=0;
  for (j=0;j<L;j++) 
    for (i=0;i<W;i++) {
      if (!LPIX[index] && (l.X(i-1,j)||l.X(i+1,j)||l.X(i,j-1)||l.X(i,j+1)))
         // Affichage des frontieres en magenta
         PIX[index] = 255 + 255*65536;//(255,0,255)
      else {
         if (LPIX[index]) {
               PIX[index] = (int)((DIST_2[index]*255.0)/(max));//niveaux de rouge
                if ((PIX[index]>2)&&((PIX[index]%50)<2)) PIX[index]=255;//(255,0,0)
         } else  {
              PIX[index] = 65536*(int)((DIST_1[index]*255.0)/(max));//niveaux de bleus
              if ((PIX[index]>2)&&((PIX[index]%50)<2)) PIX[index]=255*65536;//(0,0,255)
          }
       }
      index++;
    }
   free(DIST_1);
   free(DIST_2);
}

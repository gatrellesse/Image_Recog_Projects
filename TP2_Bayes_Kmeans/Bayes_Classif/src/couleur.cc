#include "Image.hh"
#include "couleur.hh"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void DecomposeRGB(Image<int>& p_coul,Image<int>& p_comp,int composante) {
  int index;
  int h = p_coul.PL();
  int w = p_coul.PW();
  int* PIX = p_coul.PI();
  int* PCOMP = p_comp.PI();
  
  if (composante == 1)//Rouge
    for (index=0;index<w*h;index++) PCOMP[index] = PIX[index]%256;
  else
 if (composante == 2)//Vert
    for (index=0;index<w*h;index++) PCOMP[index] = (PIX[index]>>8)%256;
  else
 if (composante == 3)//Bleu
    for (index=0;index<w*h;index++) PCOMP[index] = (PIX[index]>>16)%256;
}

void DecomposeHSV(Image<int>& p_coul,Image<int>& p_comp,int composante) {
  int index;
  int h = p_coul.PL();
  int w = p_coul.PW();
  int* PIX = p_coul.PI();
  int* PCOMP = p_comp.PI();
  int r,g,b;
  int max,min,med,m,sat;
  double dsat,dm,dmax,dmin,dmed,dhue;
  int lambda,mult;
  
   for (index=0;index<w*h;index++) {
      //Décomposition (r,g,b)
      r = PIX[index]%256;
      g = (PIX[index]>>8)%256;
      b = (PIX[index]>>16)%256;
      //Calcul des statistiques d'ordre {min,med,max}
      //Calcul du numéro de sextant (lambda)
      if (r<g) {
	if (g<b) {
	  min=r;med=g;max=b;lambda=3;mult=-1;
	} else {
	  if (r<b) {
	    min=r;med=b;max=g;lambda=2;mult=1;
	  } else {
	    min=b;med=r;max=g;lambda=1;mult=-1;
	  } 
	}
      } else {
	if (r<b) {
	  min=g;med=r;max=b;lambda=4;mult=1;
	} else {
	  if (g<b) {
	    min=g;med=b;max=r;lambda=5;mult=-1;
	  } else {
	    min=b;med=g;max=r;lambda=0;mult=1;
	  }
	}
      }
      // Calcul de l'intensité
      m =(r+g+b)/3;
      // Calcul de la saturation
      if (m>med) sat = (int)((3*(max-m))/2);
      else sat = (int)((3*(m-min))/2);
      // Calcul de la teinte
      dm = ((double)m)/255;
      dmin = ((double)min)/255;
      dmed = ((double)med)/255;
      dmax = ((double)max)/255;
      if (dm>dmed) dsat = 3*(dmax-dm)/2;
      else dsat = 3*(dm-dmin)/2;
      if (dsat =! 0) {
	dhue = (lambda + 0.5 - mult*((dmax+dmin-2*dmed)/(2*dsat)))/6;
      } else dhue=0;
      if (composante == 1) PCOMP[index] = (int)(255*dhue);
      else if (composante == 2) PCOMP[index] = sat;
      else if (composante == 3) PCOMP[index] = m;
      }
}

void DecomposeYUV(Image<int>& p_coul,Image<int>& p_comp,int composante) {
  int index;
  int h = p_coul.PL();
  int w = p_coul.PW();
  int* PIX = p_coul.PI();
  int* PCOMP = p_comp.PI();
  float r,g,b;
  float Y_f,Cr_f,Cb_f;

  for (index=0;index<w*h;index++) {
    r = (float)(PIX[index]%256);
    g = (float)((PIX[index]>>8)%256);
    b = (float)((PIX[index]>>16)%256);
       //Calcul de la luminance Y
    Y_f = 0.2989*r + 0.5866*g + 0.1145*b;
       //Calcul du contraste Bleu-Jaune Cb
    Cb_f = -0.1687*r - 0.3312*g + 0.5*b;
       //Calcul du contraste Rouge-Cyan Cr
    Cr_f = 0.5*r - 0.4183*g - 0.0817*b;
    if (composante == 1) PCOMP[index] = (int)(Y_f);
    else if (composante == 2) PCOMP[index] = 128 + (int)(Cr_f);
    else if (composante == 3) PCOMP[index] = 128 + (int)(Cb_f);
     }
}

int Get_Val(Image<int>& p,int composante,int x,int y) {
  
  int* PIX=p.PI();
  int L=p.PL();
  int W=p.PW();
  int r,g,b;
  int m,max,min,med,sat;
  double dsat,dm,dmax,dmin,dmed;
  int lambda,mult;
  float Y_f,Cr_f,Cb_f;
  int index;

  
  index = y*W + x;//Calcul index pixel
  //Calcul composantes (r,g,b)
  b = (PIX[index])/65536;
  if (composante == 3) return b; 
  g = (PIX[index]-(b*65536))/256;
  if (composante == 2) return g; 
  r = (PIX[index]-(b*65536) - g*256);
  if (composante == 1) return r; 
  m = (r+b+g)/3;
  if (composante == 6) return m;
  //Calcul composantes (Y,Cr,Cb)
  Y_f = 0.2989*r + 0.5866*g + 0.1145*b;
  if (composante == 7) return (int)(Y_f);
  Cb_f = -0.1687*r - 0.3312*g + 0.5*b;
  if (composante == 8) return 128 + (int)(Cb_f);
  Cr_f = 0.5*r - 0.4183*g - 0.0817*b;
  if (composante == 9) return 128 + (int)(Cr_f);
  if (r<g) {
    if (g<b) {
      min=r;med=g;max=b;lambda=3;mult=-1;
    } else {
      if (r<b) {
	min=r;med=b;max=g;lambda=2;mult=1;
      } else {
	min=b;med=r;max=g;lambda=1;mult=-1;
      } 
    }
  } else {
    if (r<b) {
      min=g;med=r;max=b;lambda=4;mult=1;
    } else {
      if (g<b) {
	min=g;med=b;max=r;lambda=5;mult=-1;
      } else {
	min=b;med=g;max=r;lambda=0;mult=1;
      }
    }
  }
  if (composante == 5) {
      if (m>med) return (int)((3*(max-m))/2);
      else return (int)((3*(m-min))/2);
  } else {
    dm = ((double)m)/255;
    dmin = ((double)min)/255;
    dmed = ((double)med)/255;
    dmax = ((double)max)/255;
    if (dm>dmed) dsat = 3*(dmax-dm)/2;
    else dsat = 3*(dm-dmin)/2;
    if (dsat =! 0) {
      return (int)(255*(lambda + 0.5 - mult*((dmax+dmin-2*dmed)/(2*dsat)))/6);
    } else return 0;
  }
}

void Test_Classif(Image<int>& p_col,Image<int>& p_model,Image<int>& p_classif,int sp_x,int sp_y) {
  int index,index_mod;
  int* PIX = p_col.PI();
  int* MOD = p_model.PI();
  int* PCLASSIF = p_classif.PI();
  int H = p_col.PL();
  int W = p_col.PW();
  int w_mod = p_model.PW();
  Image<int> CompX(p_col);
  Image<int> CompY(p_col);
  int* CX = CompX.PI();
  int* CY = CompY.PI();
 
  //Projection de tous les pixels dans l'espace d'observation
  if (sp_x==1) DecomposeRGB(p_col,CompX,1);
  else if (sp_x==2) DecomposeRGB(p_col,CompX,2);
  else if (sp_x==3) DecomposeRGB(p_col,CompX,3);
  else if (sp_x==4) DecomposeHSV(p_col,CompX,1);
  else if (sp_x==5) DecomposeHSV(p_col,CompX,2);
  else if (sp_x==6) DecomposeHSV(p_col,CompX,3);
  else if (sp_x==7) DecomposeYUV(p_col,CompX,1);
  else if (sp_x==8) DecomposeYUV(p_col,CompX,2);
  else if (sp_x==9) DecomposeYUV(p_col,CompX,3);
  if (sp_y==1) DecomposeRGB(p_col,CompY,1);
  else if (sp_y==2) DecomposeRGB(p_col,CompY,2);
  else if (sp_y==3) DecomposeRGB(p_col,CompY,3);
  else if (sp_y==4) DecomposeHSV(p_col,CompY,1);
  else if (sp_y==5) DecomposeHSV(p_col,CompY,2);
  else if (sp_y==6) DecomposeHSV(p_col,CompY,3);
  else if (sp_y==7) DecomposeYUV(p_col,CompY,1);
  else if (sp_y==8) DecomposeYUV(p_col,CompY,2);
  else if (sp_y==9) DecomposeYUV(p_col,CompY,3);

  //Classification selon le modele bayesien
  for (index=0;index<W*H;index++) {
    index_mod = CY[index]*w_mod + CX[index];
    if (MOD[index_mod] > 255) PCLASSIF[index] = 255*65536;//Bleu = (0,0,255)
    else PCLASSIF[index] = 255;//Rouge = (255,0,0)
   }
}

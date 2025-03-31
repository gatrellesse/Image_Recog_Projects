#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tk.h>
#include "photo.hh"
#include "Image.hh"
#include "distance.hh"
#include "couleur.hh"

/* stockage des infos d'image */

Tk_PhotoImageBlock *blockPtr;

static int TAB[NMAX][NMAX];
static int taille;

template <class T> struct infoimage{
  Image<int> img;
  Tk_PhotoHandle handle;
};

/* Conversion Format Tk --> Inti */

Image<int> tkimage2Inti(Tk_PhotoImageBlock *tkim,int composante){
       int i,j,n;
       int *pixrim;
       unsigned char *pixtk;
       int tw=tkim->width;
       int th=tkim->height;
       Image<int> p(tw,th);

       pixrim=p.PI();
       for (j=0;j<tkim->height;j++) {
	 pixtk=tkim->pixelPtr + tkim->pitch*j;
          for (i=0;i<tkim->width;i++) {
	    if (composante > 0) {//Cas d'une composante (image en niveau de gris)
	      n = (int)(*(pixtk+tkim->offset[composante-1]));
	    } else {//Cas d'une image couleur : entier 24 bits
	      n = (int)(*(pixtk+tkim->offset[0])) 
		+ ((int)(*(pixtk+tkim->offset[1])))*256
		+ ((int)(*(pixtk+tkim->offset[2])))*65536;
	    }
	    *pixrim = n;
	    pixtk+=tkim->pixelSize;pixrim++;
          }
       }
       return p;
}

/* Conversions Format Inti --> Tk */

Tk_PhotoImageBlock *Inti2tkimage_gray(Image<int>& pp,Tk_PhotoImageBlock *tkim) {
       unsigned char *tempotk;
       int *tempoplan;
       int i;
       if (tkim == NULL) {
              tkim = (Tk_PhotoImageBlock *)malloc(sizeof(Tk_PhotoImageBlock));
	      tkim->pixelPtr = NULL;
       }

       tkim->width=pp.PW();
       tkim->height=pp.PL();
       tkim->pixelSize=3;
       tkim->pitch=tkim->width*tkim->pixelSize;
       tkim->offset[0]=0;
       tkim->offset[1]=1;
       tkim->offset[2]=2;

       if (tkim->pixelPtr == NULL) {
	 tempotk= tkim->pixelPtr=(unsigned char *)malloc(tkim->pixelSize*tkim->width*tkim->height*sizeof(unsigned char));
       } else {
	 tempotk= tkim->pixelPtr=(unsigned char *)realloc(tkim->pixelPtr,tkim->pixelSize*tkim->width*tkim->height*sizeof(unsigned char));
       }
       tempoplan=pp.PI();
       for (i=0;i<tkim->width*tkim->height;i++) {
	 *tempotk = *(tempotk+tkim->offset[1]) = *(tempotk+tkim->offset[2]) = (unsigned char)(*tempoplan);
	 tempoplan++;
	 tempotk+=tkim->pixelSize;
       }
       return tkim;
}

Tk_PhotoImageBlock *Inti2tkimage_color(Image<int>& pp,Tk_PhotoImageBlock *tkim) {
       unsigned char *tempotk;
       int *tempoplan;
       int i;
       if (tkim == NULL) {
              tkim = (Tk_PhotoImageBlock *)malloc(sizeof(Tk_PhotoImageBlock));
	      tkim->pixelPtr = NULL;
       }

       tkim->width=pp.PW();
       tkim->height=pp.PL();
       tkim->pixelSize=3;
       tkim->pitch=tkim->width*tkim->pixelSize;
       tkim->offset[0]=0;
       tkim->offset[1]=1;
       tkim->offset[2]=2;

       if (tkim->pixelPtr == NULL) {
	 tempotk= tkim->pixelPtr=(unsigned char *)malloc(tkim->pixelSize*tkim->width*tkim->height*sizeof(unsigned char));
       } else {
	 tempotk= tkim->pixelPtr=(unsigned char *)realloc(tkim->pixelPtr,tkim->pixelSize*tkim->width*tkim->height*sizeof(unsigned char));
       }
       tempoplan=pp.PI();
       for (i=0;i<tkim->width*tkim->height;i++) {
	 *(tempotk+tkim->offset[0]) = (*tempoplan)%256; 
	 *(tempotk+tkim->offset[1]) = ((*tempoplan)>>8)%256;
	 *(tempotk+tkim->offset[2]) = ((*tempoplan)>>16)%256;
	 tempoplan++;
	 tempotk+=tkim->pixelSize;
       }
       return tkim;
}


void freetkim(Tk_PhotoImageBlock *tkim) {
       free(tkim->pixelPtr);
       free(tkim);
}

int Inti_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
       infoimage<int>* cdata;
       cdata  = (infoimage<int>*) clientData;
       
       int x,y,x1,x2,y1,y2;
       int comp,comp1,comp2,chg;
       char *numb;
       float det1,a1,b1,c1,det2,a2,b2,c2,p;
       
       if (strcmp(argv[1],"euclide") == 0) {
         x1 = atoi(argv[2]);y1 = atoi(argv[3]);
	 Euclide(cdata[1].img,x1,y1);chg=2;}
       else 
       if (strcmp(argv[1],"euclide2") == 0) {
         x1 = atoi(argv[2]);y1 = atoi(argv[3]);
         x2 = atoi(argv[4]);y2 = atoi(argv[5]);
	 Euclide2(cdata[1].img,x1,y1,x2,y2);chg=2;}
       else 
       if (strcmp(argv[1],"mahalanobis") == 0) {
           x1 = atoi(argv[2]);y1 = atoi(argv[3]);
	   a1 = atof(argv[4]);b1 = atof(argv[5]);c1 = atof(argv[6]);
	   Mahalanobis(cdata[1].img,x1,y1,a1,b1,c1);chg=2;}
       else
       if (strcmp(argv[1],"bayes_cloud") == 0) {
          x1 = atoi(argv[2]);y1 = atoi(argv[3]);det1 = atof(argv[4]);
	  a1 = atof(argv[5]);b1 = atof(argv[6]);c1 = atof(argv[7]);
          x2 = atoi(argv[8]);y2 = atoi(argv[9]);det2 = atof(argv[10]);
	  a2 = atof(argv[11]);b2 = atof(argv[12]);c2 = atof(argv[13]);
          p = atof(argv[14]);
	  Bayes_Cloud(cdata[1].img,x1,y1,det1,a1,b1,c1,x2,y2,det2,a2,b2,c2,p);chg=2;}
       else
       if (strcmp(argv[1],"test_rgb") == 0) {
          comp = atoi(argv[2]);
          DecomposeRGB(cdata[0].img,cdata[2].img,comp);chg=1;}
       else
       if (strcmp(argv[1],"test_hsv") == 0) {
          comp = atoi(argv[2]);
          DecomposeHSV(cdata[0].img,cdata[2].img,comp);chg=1;}
       else
       if (strcmp(argv[1],"test_yuv") == 0) {
          comp = atoi(argv[2]);
          DecomposeYUV(cdata[0].img,cdata[2].img,comp);chg=1;}
       else 
       if (strcmp(argv[1],"getvalue") == 0) {
	   numb = (char*)malloc(10*sizeof(char));
	   comp = atoi(argv[2]);x = atoi(argv[3]);y = atoi(argv[4]);
	   sprintf(numb,"%d",Get_Val(cdata[0].img,comp,x,y));
	   Tcl_AppendElement(interp,numb);
	   chg=0;} 
       else
       if (strcmp(argv[1],"test_classif") == 0) {
          comp1 = atoi(argv[2]); comp2 = atoi(argv[3]);
          Test_Classif(cdata[0].img,cdata[1].img,cdata[2].img,comp1,comp2);chg=11;}
       if (chg==1) {
         blockPtr=Inti2tkimage_gray(cdata[2].img,blockPtr);
         Tk_PhotoPutBlock(cdata[2].handle, blockPtr, 0, 0, blockPtr->width, blockPtr->height);
      } else if (chg==2) {
         blockPtr=Inti2tkimage_color(cdata[1].img,blockPtr);
         Tk_PhotoPutBlock(cdata[1].handle, blockPtr, 0, 0, blockPtr->width, blockPtr->height);
        } else if (chg==11) {
         blockPtr=Inti2tkimage_color(cdata[2].img,blockPtr);
         Tk_PhotoPutBlock(cdata[2].handle, blockPtr, 0, 0, blockPtr->width, blockPtr->height);
        }
       return TCL_OK;       
}

int img_new(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
       infoimage<int> *cdata;
       Tk_PhotoImageBlock blockPtr;
       char *error_msg;
       int x = atoi(argv[2]);

       cdata = (infoimage<int>*) clientData;
     if (argc != 3) {
                sprintf(error_msg,"Usage: Imgnew <image> <numero>");
                Tcl_SetResult(interp,error_msg,TCL_VOLATILE);
		return TCL_ERROR;
	}
   cdata[x].handle=Tk_FindPhoto(interp,argv[1]);
   Tk_PhotoGetImage(cdata[x].handle, &blockPtr);
   cdata[x].img=tkimage2Inti(&blockPtr,x);
 return TCL_OK;
}

/* creation d'une image de type Inti */
/* Intinew <nomimage> <nomcommanderet> */

void init_retcc() {
blockPtr = NULL;
}

int Inti_new(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
       infoimage<int> *cdata;
       char *error_msg;

       if (argc != 3) {
         sprintf(error_msg,"Usage: Intinew <nomcommande> <nb_images>");
         Tcl_SetResult(interp,error_msg,TCL_VOLATILE);
	 return TCL_ERROR;
       }
       int nbimages = atoi(argv[2]);
       cdata = new infoimage<int>[nbimages];
       Tcl_CreateCommand(interp, "Imgnew", img_new, (ClientData)cdata,(Tcl_CmdDeleteProc *)NULL);
       Tcl_CreateCommand(interp, argv[1], Inti_cmd, (ClientData)cdata,(Tcl_CmdDeleteProc *)NULL);
       return TCL_OK;
}


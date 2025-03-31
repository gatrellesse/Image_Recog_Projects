#ifndef _DIST_
#define _DIST_

void Euclide(Image<int>& p,int moy_x,int moy_y);
void Euclide2(Image<int>& p,int moy1_x,int moy1_y,int moy2_x,int moy2_y);
void Mahalanobis(Image<int>& p,int moy_x,int moy_y,float a11,float a21,float a22);
void Bayes_Cloud(Image<int>& p,int moy1_x,int moy1_y,float det1,float a11,float a21,float a22,
 			int moy2_x,int moy2_y,float det2,float b11,float b21,float b22,float proba_c1);

#endif

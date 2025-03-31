#ifndef _COULEUR_
#define _COULEUR_

void DecomposeRGB(Image<int>& p_coul,Image<int>& p_comp,int composante);
void DecomposeHSV(Image<int>& p_coul,Image<int>& p_comp,int composante);
void DecomposeYUV(Image<int>& p_coul,Image<int>& p_comp,int composante);

int Get_Val(Image<int>& p,int composante,int x,int y);

void Test_Classif(Image<int>& p_col,Image<int>& p_model,Image<int>& p_classif,int sp_x,int sp_y);

#endif

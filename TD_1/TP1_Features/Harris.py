import numpy as np
import cv2

from matplotlib import pyplot as plt

#Lecture image en niveau de gris et conversion en float64
img=np.float64(cv2.imread('../Image_Pairs/Graffiti0.png',cv2.IMREAD_GRAYSCALE))
(h,w) = img.shape
print("Dimension de l'image :",h,"lignes x",w,"colonnes")
print("Type de l'image :",img.dtype)
sigmas = [1, 2, 3200]

fig, axes = plt.subplots(len(sigmas), 3, figsize=(12, 2.5 * len(sigmas)))

#Début du calcul
for i, s in enumerate(sigmas):
    t1 = cv2.getTickCount()
    Theta = cv2.copyMakeBorder(img,0,0,0,0,cv2.BORDER_REPLICATE)
    # Mettre ici le calcul de la fonction d'intérêt de Harris
    #Sobel
    kernel_x = np.array([[-1, 0, 1],
                    [-2, 0, 2],
                    [-1, 0, 1]], dtype=np.float32)

    kernel_y = np.array([[-1, -2, -1],
                    [ 0,  0,  0],
                    [ 1,  2,  1]], dtype=np.float32)

    #Calcul des Gradients
    Ix = cv2.filter2D(img, -1, kernel_x)
    Iy = cv2.filter2D(img, -1, kernel_y)

    #Gaussian fenetre pour reduire le bruit
    #Nous pouvons creer un gaussian kernel et faire des conv 
    #ou utiliser la function du cv2
    window_size = 7  # Taille de la fenetre gaussiane
    kernel_size = (window_size, window_size)
    alpha = 0.04  # Ponderation parameter [0.04, 0.06]
    sigma = np.sqrt(2 * s)
    #sigma_Y = sigma_x par default
    Ixx = cv2.GaussianBlur(Ix * Ix, kernel_size, sigmaX = sigma)
    Iyy = cv2.GaussianBlur(Iy * Iy, kernel_size, sigmaX = sigma)
    Ixy = cv2.GaussianBlur(Ix * Iy, kernel_size, sigmaX = sigma)
    

    #Compute the Harris response function (Theta)
    det_M = Ixx * Iyy - Ixy**2  # Determinant of M
    trace_M = Ixx + Iyy  # Trace of M
    Theta = det_M - alpha * (trace_M**2)  # Harris response
    


    # Calcul des maxima locaux et seuillage
    Theta_maxloc = cv2.copyMakeBorder(Theta,0,0,0,0,cv2.BORDER_REPLICATE)
    d_maxloc = 3
    seuil_relatif = 0.01
    se = np.ones((d_maxloc,d_maxloc),np.uint8)
    Theta_dil = cv2.dilate(Theta,se)

    #Suppression des non-maxima-locaux
    Theta_maxloc[Theta < Theta_dil] = 0.0

    #On néglige également les valeurs trop faibles
    Theta_maxloc[Theta < seuil_relatif*Theta.max()] = 0.0
    t2 = cv2.getTickCount()
    time = (t2 - t1)/ cv2.getTickFrequency()
    print("Mon calcul des points de Harris :",time,"s")
    print("Nombre de cycles par pixel :",(t2 - t1)/(h*w),"cpp")

    # Affichage des résultats
    axes[i, 0].imshow(img, cmap='gray')
    axes[i, 0].set_title(f'Image originale ')

    axes[i, 1].imshow(Theta, cmap='gray')
    axes[i, 1].set_title(f'Fonction de Harris (σ={s})')


    se_croix = np.uint8([[1, 0, 0, 0, 1],
                        [0, 1, 0, 1, 0],
                        [0, 0, 1, 0, 0],
                        [0, 1, 0, 1, 0],
                        [1, 0, 0, 0, 1]])
    Theta_ml_dil = cv2.dilate(Theta_maxloc,se_croix)
    number_points = np.sum(Theta_ml_dil > 0)
    #Relecture image pour affichage couleur
    Img_pts=cv2.imread('../Image_Pairs/Graffiti0.png',cv2.IMREAD_COLOR)
    (h,w,c) = Img_pts.shape
    print("Dimension de l'image :",h,"lignes x",w,"colonnes x",c,"canaux")
    print("Type de l'image :",Img_pts.dtype, " Number of points : ", number_points,"\n")
    #On affiche les points (croix) en rouge
    Img_pts[Theta_ml_dil > 0] = [255,0,0]
    #Normalizing des valeurs negatives et positives
    Img_pts = cv2.normalize(Img_pts, None, 0, 255, cv2.NORM_MINMAX)

    axes[i, 2].imshow(cv2.cvtColor(Img_pts, cv2.COLOR_BGR2RGB))
    axes[i, 2].set_title(f'Points de Harris (σ={s})' )

# Ajustement et affichage final
plt.tight_layout()
plt.show()

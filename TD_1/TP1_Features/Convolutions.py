import numpy as np
import cv2

from matplotlib import pyplot as plt

#Lecture image en niveau de gris et conversion en float64
img=np.float64(cv2.imread('../Image_Pairs/FlowerGarden2.png',0))
(h,w) = img.shape
print("Dimension de l'image :",h,"lignes x",w,"colonnes")

#Méthode directe
t1 = cv2.getTickCount()
img2 = cv2.copyMakeBorder(img,0,0,0,0,cv2.BORDER_REPLICATE)
for y in range(1,h-1):
  for x in range(1,w-1):
    val = 5*img[y, x] - img[y-1, x] - img[y, x-1] - img[y+1, x] - img[y, x+1] 
    img2[y,x] = min(max(val,0),255)
t2 = cv2.getTickCount()
time = (t2 - t1)/ cv2.getTickFrequency()
print("Méthode directe :",time,"s")

cv2.imshow('Avec boucle python',img2.astype(np.uint8))
#Convention OpenCV : une image de type entier est interprétée dans {0,...,255}
cv2.waitKey(0)

plt.subplot(121)
plt.imshow(img2,cmap = 'gray')
plt.title('Convolution - Méthode Directe')

#Méthode filter2D
#Shapen kernel: [[0, -1, 0],[-1, 5, -1],[0, -1, 0]]
t1 = cv2.getTickCount()
kernel = np.array([[0, -1, 0],[-1, 5, -1],[0, -1, 0]])
img3 = cv2.filter2D(img,-1,kernel)
t2 = cv2.getTickCount()
time = (t2 - t1)/ cv2.getTickFrequency()
print("Méthode filter2D :",time,"s")

cv2.imshow('Avec filter2D',img3/255.0)
#Convention OpenCV : une image de type flottant est interprétée dans [0,1]
cv2.waitKey(0)

plt.subplot(122)
plt.imshow(img3,cmap = 'gray',vmin = 0.0,vmax = 255.0)
#Convention Matplotlib : par défaut, normalise l'histogramme !
plt.title('Convolution - filter2D')

plt.show()

#Question 3
#Pour efectuer des operations de derivation horizontale et verticale
#on utiliserai des operateurs de Sobel
# Définir les noyaux de Sobel pour calculer les gradients
kernel_x = np.array([[-1, 0, 1],
                    [-2, 0, 2],
                    [-1, 0, 1]], dtype=np.float32)

kernel_y = np.array([[-1, -2, -1],
                    [ 0,  0,  0],
                    [ 1,  2,  1]], dtype=np.float32)

#Calcul des Gradients et la norme
Ix = cv2.filter2D(img, -1, kernel_x)
Iy = cv2.filter2D(img, -1, kernel_y)
gradient_norm = np.sqrt(Ix**2 + Iy**2)
#print(Ix,Iy,gradient_norm)

Ix_norm = (cv2.normalize(Ix, None, 0, 255, cv2.NORM_MINMAX)).astype(np.uint8)
Iy_norm = (cv2.normalize(Iy, None, 0, 255, cv2.NORM_MINMAX)).astype(np.uint8)
gradient_norm_norm = (cv2.normalize(gradient_norm, None, 0, 255, cv2.NORM_MINMAX)).astype(np.uint8)

# Afficher les résultats
plt.figure(figsize=(12, 8))

plt.subplot(2, 2, 1)
plt.imshow(img, cmap='gray')
plt.title('Image originale')
plt.axis('off')

plt.subplot(2, 2, 2)
plt.imshow(Ix_norm, cmap='gray')
plt.title('Gradient horizontal (Ix)')
plt.axis('off')

plt.subplot(2, 2, 3)
plt.imshow(Iy_norm, cmap='gray')
plt.title('Gradient vertical (Iy)')
plt.axis('off')

plt.subplot(2, 2, 4)
plt.imshow(gradient_norm_norm, cmap='gray')
plt.title('Norme euclidienne du gradient (||∇I||)')
plt.axis('off')

plt.tight_layout()
plt.show()

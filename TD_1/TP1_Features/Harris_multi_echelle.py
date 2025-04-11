import numpy as np
import cv2

from matplotlib import pyplot as plt
from scipy.ndimage import maximum_filter
from scipy.spatial import distance

#Lecture image en niveau de gris et conversion en float64
img=np.float64(cv2.imread('Image_Pairs\\Graffiti0.png',cv2.IMREAD_GRAYSCALE))
(h,w) = img.shape
print("Dimension de l'image :",h,"lignes x",w,"colonnes")
print("Type de l'image :",img.dtype)

#Paramètres
sigmas = [1, 2, 7, 10]
alpha = 0.04
window_size = 7
r = 10  # Distance minimale entre les points-clés

Theta_stack = []

#Début du calcul
# Calcul de la fonction de Harris à plusieurs échelles
for s in sigmas:
    img_blur = cv2.GaussianBlur(img, (0, 0), sigmaX=s)
    Ix = cv2.Sobel(img_blur, cv2.CV_64F, 1, 0, ksize=3)
    Iy = cv2.Sobel(img_blur, cv2.CV_64F, 0, 1, ksize=3)

    sigma_int = np.sqrt(2 * s)
    Ixx = cv2.GaussianBlur(Ix * Ix, (window_size, window_size), sigmaX=sigma_int)
    Iyy = cv2.GaussianBlur(Iy * Iy, (window_size, window_size), sigmaX=sigma_int)
    Ixy = cv2.GaussianBlur(Ix * Iy, (window_size, window_size), sigmaX=sigma_int)

    det_M = Ixx * Iyy - Ixy**2
    trace_M = Ixx + Iyy
    Theta = det_M - alpha * (trace_M**2)

    Theta_stack.append(Theta)

# Création du volume 3D (x, y, σ)
Theta_volume = np.stack(Theta_stack, axis=-1)  # (H, W, num_scales)

# Suppression des non-maxima en espace-échelle
nms_volume = maximum_filter(Theta_volume, size=(3, 3, 3))
keypoints_mask = (Theta_volume == nms_volume) & (Theta_volume > 0.01 * Theta_volume.max())
keypoints = np.argwhere(keypoints_mask)  # (y, x, s_idx)
print(f"{len(keypoints)} multi-scale Harris points detected before spatial suppression.")

# Appliquer la suppression avec distance minimale r
keypoints_full = [
    (y, x, Theta_volume[y, x, s_idx], s_idx)
    for (y, x, s_idx) in keypoints
]
keypoints_full.sort(key=lambda k: -k[2])  # Trier par réponse décroissante

selected = []
for kp in keypoints_full:
    y, x, _, s_idx = kp
    if all(np.hypot(x - x2, y - y2) >= r for (y2, x2, _, _) in selected):
        selected.append(kp)

print(f"Keypoints after enforcing r={r} pixel spacing: {len(selected)}")

# Affichage
img_rgb = cv2.imread('Image_Pairs\\Graffiti0.png')
for y, x, _, s_idx in selected:
    sigma = sigmas[s_idx]
    cv2.circle(img_rgb, (x, y), int(max(1, sigma)), (0, 0, 255), 1)

plt.figure(figsize=(10, 8))
plt.imshow(cv2.cvtColor(img_rgb, cv2.COLOR_BGR2RGB))
plt.title(f"Points de Harris multi-échelles (distance ≥ {r} pixels)")
plt.axis('off')
plt.show()
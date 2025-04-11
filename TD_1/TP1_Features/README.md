# TD1

## Question 1

#### Comparaison des performances
Nous observons une différence significative en termes de temps de calcul entre les deux méthodes. En utilisant le filtre 2D d'OpenCV, nous obtenons des résultats environ **100 fois plus rapides** que la méthode directe.

#### Explication de l'optimisation
- **Petits noyaux** : OpenCV optimise la convolution en exploitant la décomposition d'un noyau $ k = r \times c $, réduisant ainsi la complexité de $ O(MNPQ) $ à $ O(MN(P+Q)) $.
- **Grands noyaux** : Il utilise le **théorème de convolution**, qui convertit la convolution dans le domaine spatial en une multiplication dans le domaine fréquentiel, avec une implémentation FFT efficace :
  
  $$ f * g = \mathcal{F}^{-1}(\mathcal{F}(f) \cdot \mathcal{F}(g)) $$
  
  Ce qui réduit la complexité de $ O(MNPQ) $ à $ O((M+K) \cdot (N+L) \cdot \log(M+K) \cdot \log(N+L)) $.

#### Fonctions de lecture et d'affichage
- **cv2.imread()** : Prend l'adresse du fichier de l'image et une flag comme entrées. Les flags indiquent comme la fonction chargera l'image, par exemple, en niveaux de gris (0), en couleur (1) format BGR, sans ou avec canal alpha, etc. L'image sera décodifié par un certain module (construit en C++) en accord avec son format - libjpeg ou libpng, par exemple. La sortie de cette fonction est un tableau numpy de type uint8 quit contient (hauteur, largeur) pour les images en niveaux de gris ou (hauteur, largeur, 3) pour les images en couleur.

- **cv2.copyMakeBorder()** : cette fonction a pour but principal ajouter un "padding" autour de l'image. L'idée c'est de rendre possible les cas où des opérations de convolution accèdent à des pixels voisins qui n'existent pas en bordure. Les entrées de la fonction sont l'image originale, les nombres de pixels qui seront ajoutés à gauche, à droite, en haut et en bas et un argument pour le type de bordure à ajouter. Bien sûr, si vous voulez juste faire une copie des images, vous pouvez utiliser copyMakeBorder() juste remplaçant chaque argument de bordure par 0 et en ajoutant cv2.BORDER_REPLICATE comme dernier argument.

- **cv2.imshow()** : cette fonction utilise les API graphiques natives du système d'exploitation, comme Win32 sur Windows ou GTK/Qt sur Linux. Le tableau numpy de l'image est convertit en une bitmap compatible pour l'affichage. Ce tableau .np d'entrée doit être sur le format uint8 pour éviter des problèmes d'affichage. La fonction cv2.waitKey() utilisé juste après garde la fenêtre ouverte.

- **plt.imshow()** : la fonction de matplotlib pour l'affichage utilise le moteur de rendu de matplotlib. Différement de la fonction de cv2, celle-ci accepte des tableaux numpy de n'importe quel format, et fait la normalisation si nécessaire. Elle permet aussi l'utilisation de color maps. Il faut savoir que cette fonction utilise le format RGB pour les couleurs, donc si vous avez utilisé OpenCV pour lire l'image en couleur, vous devez convertir avec la fonction cv2.cvtColor().

## Question 2

Donc, si on compare ce noyau avec le noyau de convolution fourni en exemple dans la routine **Convolutions.py**, on note que l'élément central est multiplié par 5 au lieu de 4 - et cela induit un réhaussement de contraste - surtout pour les bords.

D'abord, on remarque que le noyau correspondant au Laplacien en 4-connexité est égal à:
$$
  \begin{bmatrix} 
  0 &-1 & 0 \\ 
  -1 & 4 &-1 \\ 
  0 &-1 & 0
  \end{bmatrix}
$$
 
Cette opérateur quantifie l'intensité de la variation des niveaux de gris autour d'un pixel, c'est-à-dire, il mesure le contraste.

## Question 3

Puisque la convolution approxime les composantes des gradients des dérivées partielles, nous allons utiliser les opérateurs de Sobel pour calculer $ I_x $, $ I_y $ et, par conséquent, la norme du gradient.  

Il faut faire attention à l'affichage, car ces trois résultats peuvent contenir des valeurs négatives ou des valeurs positives hors de l'intervalle utilisé par les fonctions de Matplotlib, qui attendent des valeurs entre **[0, 255]**. Par conséquent, nous allons normaliser ces valeurs dans cet intervalle.  

Une dernière remarque sur le type de données du gradient : comme il peut contenir des valeurs négatives, l'utilisation du type **uint8** peut poser un problème. Il est donc préférable d'utiliser **float32** avant la normalisation afin de préserver la précision des calculs.

## Question 4

La valeur de θ nous indique si la direction du pixel est classifiée comme plat (flat), coin (corner) ou bord (edge).

- Si θ est fortement positif, le pixel est classé comme un coin (corner).
- Si θ est négatif, il est classé comme un bord (edge).
- Si θ est légèrement positif, il est classé comme une région plate (flat).

Cela représente uniquement l'analyse de θ. Sa valeur provient principalement du calcul du gradient avec le filtre de Sobel, qui permet d'identifier les variations selon les axes X et Y. Ensuite, un filtre gaussien est appliqué pour réduire le bruit (blur). Enfin, nous calculons les valeurs propes de la matrice M, qui nous aidera à déterminer si un point correspond à un coin (corner) ou non. 

https://medium.com/@itberrios6/harris-corner-and-edge-detector-4169312aa2f8

## Question 5


+alpha, -points --> Theta va diminuer --> Moins de points que sont plus important que le treshold
+taille fenetre, - points --> Simpler images generated, less blur to exclude
+s, + points --> **Nao sei, n parece estar tendo mt efeito igual ao livro, precisa reve**

- **$\alpha$ = 0.01** : 590 points multi-échelle;
- **$\alpha$ = 0.04** : 541 points multi-échelle;
- **$\alpha$ = 0.06** : 541 points multi-échelle;
- **$\alpha$ = 0.08** : 481 points multi-échelle;

#### Restriction spatial (r pixels):
Pour imposer une distance minimale après la détection des points-clés avec la notion de maxima locaux, on peut ordonner les points-cles par intensité de la réponse d'après Harris et, avec une boucle, trier les points avec une distance minimale de r pixels. Pour cela, on peut utiliser la fonction distance de la bibliothèque scipy.spatial, en développant un algorithme similaire à ce qui est utilisé dans SIFT, par exemple.

## Question 6 FEITO

#### KAZE

La détection des points d'intérêt est réalisée en utilisant une technique de diffusion anisotropique. Contrairement à un filtre linéaire (comme le filtre gaussien), cette approche repose sur un filtre non linéaire dépendant du contenu de l'image. Elle exploite la variation d'un champ pour diffuser les valeurs principalement dans les régions où les variations de l'image sont faibles, tout en préservant les contours et les arêtes.

Ce filtre est appliqué à différentes échelles, générant des images de plus en plus floues. Ensuite, le déterminant du Hessien est calculé pour identifier les points d'intérêt.

Enfin, pour garantir l'invariance à la rotation, KAZE détermine l'orientation dominante des voisins (M-SURF) en calculant les dérivées partielles horizontale et verticale.


**Principaux paramètres** :

- **Threshold** : Filtre les points avec un déterminant du Hessien faible (plus élevé = moins de bruit).
- **nOctaves** : Nombre de niveaux d'échelle de la pyramide (plus élevé = plus robuste).
- **nOctaveLayers** : Nombre de sous-niveaux par octave (plus élevé = plus détaillé).
- **Diffusivity** : Type de diffusion utilisé.
KAZE article: https://www.doc.ic.ac.uk/~ajd/Publications/alcantarilla_etal_eccv2012.pdf

#### ORB:

La détection des points d'intérêt candidats est réalisée en utilisant l’algorithme FAST (Features from Accelerated Segment Test), qui repose simplement sur une comparaison de l'intensité d'un pixel avec celle de ses voisins situés sur un cercle.  

L'algorithme ne réalise pas d'analyse multi-échelle, mais utilise plutôt des couches multiples de features générées par la méthode de pyramide d’images.  

Ensuite, pour classer les points détectés, l’algorithme applique le détecteur de Harris afin d'identifier les maxima locaux.  

Enfin, pour assurer l’invariance à la rotation, un centre d’intensité(rBRIEF)est calculé à l’aide des moments d’image, ce qui permet de générer un vecteur orienté du coin détecté vers le centre de ce centroïde.  

**Principaux paramètres** :
- **nfeatures** : Nombre de features à retenir (plus grand = plus robuste, plus petit = plus précis).  
- **scaleFactor** : Facteur d’échelle entre les niveaux de la pyramide (plus grand = exécution plus rapide avec un filtrage plus grossier, plus petit = plus détaillé).  
- **nlevels** : Nombre de niveaux dans la pyramide (plus élevé = plus d’échelles donc plus robuste).

ORB article: https://www.researchgate.net/publication/221111151_ORB_an_efficient_alternative_to_SIFT_or_SURF*

#### Repetabilité visuel

Pour évaluer la répétabilité sur une même image sous différentes rotations, il faut vérifier si le modèle retrouve les mêmes points d'intérêt aux mêmes coordonnées que dans l'image originale.  

On peut également analyser le gradient d'orientation : si l'image est tournée, l'orientation des arêtes doit être ajustée en conséquence.

## Question 7

Comme bien expliqué dans l'exercice précédent, les deux détecteurs et descripteurs sont les suivants :

**Invariance à l'échelle** 
#### ORB :

- Utilise une pyramide d'images pour détecter des points à différentes échelles.
 
- La détection multi-échelle garantit que les points sont trouvés à différentes résolutions.

#### KAZE :

- Utilise un espace d'échelle non linéaire pour détecter des points à différentes échelles.

- Le descripteur est calculé à partir de régions normalisées dans l'espace d'échelle, ce qui le rend intrinsèquement invariant à l'échelle.

**Invariance à la rotation**
#### ORB :

- Utilise le centroïde d'intensité pour calculer l'orientation du point.

- Le descripteur rBRIEF est aligné selon cette orientation, ce qui le rend invariant à la rotation.

#### KAZE :

- Utilise les gradients locaux pour calculer l'orientation du point.

- Le descripteur M-SURF est aligné selon cette orientation, ce qui le rend invariant à la rotation.

## Question 8
En expérimentant les trois matchers : BFMatcher.match(), flann.knnMatch() et BFMatcher.knnMatch(), on remarque des résultats très distincts.  

Pour l’ORB, les matchers utilisant Brute-Force donnent de meilleurs résultats que celui utilisant FLANN, qui repose sur le K-nearest neighbors (KNN). Ce dernier n'est pas une bonne méthode de comparaison pour un vecteur binaire, mais il fonctionne bien pour les descripteurs en nombres flottants. Ainsi, Brute-Force avec best match est la meilleure méthode pour ORB, suivi de Brute-Force avec KNN.  

Quant à KAZE, les résultats sont globalement très similaires. Brute-Force avec match reste la meilleure option, mais les performances de KNN se sont nettement améliorées par rapport à ORB, car KAZE travaille avec des descripteurs en nombres flottants.


Les deux descripteurs utilisent des calculs de distance différents, car chacun retourne un type de variable différent avec la fonction `cv2.detectAndCompute`.  
KAZE retourne un vecteur de 64 valeurs en virgule flottante, tandis que ORB renvoie un vecteur binaire de 256 bits.  
Cela signifie que pour calculer la distance entre deux points avec KAZE, il faut utiliser la **norme euclidienne (Norm_L2)** :
$$d = \sqrt{\sum_{i=1}^{n} (x_i - y_i)^2}$$

où $ x_i $ et $ y_i $ sont les valeurs des descripteurs pour deux points.


En revanche, pour calculer la distance entre deux points avec **ORB**, il faut utiliser la **distance de Hamming (Norm_Hamming)** : 

$$d = \sum_{i=1}^{256} (x_i \oplus y_i)$$

où $ \oplus $ représente l'opérateur XOR bit à bit.

## Question 9

Le code **defformation.py** a été développé en utilisant les codes **Features_Match_RatioTest.py** et **Features_Match_CrossCheck.py** comme références. On l'a refactorisé et l'on a ajouté deux fonctions pour qu'on puisse évaluer quantitativement les effets de la déformation (rotation + translation): les routines `transform_image()` et `evaluate_match()`.

Dans cette implémentation, nous avons évalué quantitativement la robustesse des appariements de points d’intérêt en appliquant une transformation géométrique contrôlée à l’image d’origine. Pour ce faire, nous avons utilisé la fonction `cv2.warpAffine()` de la bibliothèque OpenCV, qui permet d’appliquer une transformation affine quelconque à l’image. Cette fonction prend en entrée une image, une matrice de transformation affine `M ∈ ℝ²ˣ³`, et retourne l’image transformée. L’utilisation de `cv2.getRotationMatrix2D()` nous a permis de construire directement cette matrice `M`, en assurant que l’origine de la rotation soit centrée sur l’image.

L’intérêt de cette transformation synthétique est qu’elle est connue a priori, ce qui permet de mesurer précisément l’erreur entre les points appariés et leur position attendue dans l’image transformée.

Pour évaluer la qualité des appariements, nous avons implémenté une méthode fondée sur la comparaison géométrique : chaque point détecté dans l’image originale est transformé via la matrice `M`, et comparé à sa correspondance estimée dans l’image transformée. La distance euclidienne entre ces deux positions est ensuite calculée. Si cette distance est inférieure à un certain seuil (ici 3 pixels), l’appariement est considéré comme correct. Cette métrique permet de calculer deux indicateurs essentiels : le taux d’inliers (proportion de bons appariements) et l’erreur moyenne sur les correspondances.

Ce choix méthodologique repose sur la volonté de disposer d’un critère quantitatif, reproductible et indépendant des visualisations subjectives. Il permet notamment de comparer objectivement différents détecteurs (`ORB`, `KAZE`) et stratégies d’appariement (`cross-check`, `ratio test`), en mettant en évidence leur stabilité vis-à-vis des transformations géométriques.


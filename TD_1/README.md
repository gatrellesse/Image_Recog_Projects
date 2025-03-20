# TD1

## Question 1 POR FAZER(Falar sobre as funcoes do matplot)

#### Comparaison des performances
Nous observons une différence significative en termes de temps de calcul entre les deux méthodes. En utilisant le filtre 2D d'OpenCV, nous obtenons des résultats environ **100 fois plus rapides** que la méthode directe.

#### Explication de l'optimisation
- **Petits noyaux** : OpenCV optimise la convolution en exploitant la décomposition d'un noyau $ k = r \times c $, réduisant ainsi la complexité de $ O(MNPQ) $ à $ O(MN(P+Q)) $.
- **Grands noyaux** : Il utilise le **théorème de convolution**, qui convertit la convolution dans le domaine spatial en une multiplication dans le domaine fréquentiel, avec une implémentation FFT efficace :
  
  $$ f * g = \mathcal{F}^{-1}(\mathcal{F}(f) \cdot \mathcal{F}(g)) $$
  
  Ce qui réduit la complexité de $ O(MNPQ) $ à $ O((M+K) \cdot (N+L) \cdot \log(M+K) \cdot \log(N+L)) $.

## Question 2 FEITO

Le noyau effectue un **rehaussement de contraste**, mais en réalité, il s'agit simplement de la suppression du flou(blur) de l'image originale.  

Le kernel utilisé est obtenu en appliquant la formule suivante :  $\text{résultat} = \text{original} + (\text{original} - \text{blur}) \times \text{facteur}$ où le flou est généré à l'aide d'un filtre non linéaire, plus précisément le **filtre gaussien**, qui agit comme un **filtre passe-bas**.

## Question 3 FEITO

#### O RESULTADO IX IY E NORM ta sendo plotado, colocar no rapport dps

Puisque la convolution approxime les composantes des gradients des dérivées partielles, nous allons utiliser les opérateurs de Sobel pour calculer $ I_x $, $ I_y $ et, par conséquent, la norme du gradient.  

Il faut faire attention à l'affichage, car ces trois résultats peuvent contenir des valeurs négatives ou des valeurs positives hors de l'intervalle utilisé par les fonctions de Matplotlib, qui attendent des valeurs entre **[0, 255]**. Par conséquent, nous allons normaliser ces valeurs dans cet intervalle.  

Une dernière remarque sur le type de données du gradient : comme il peut contenir des valeurs négatives, l'utilisation du type **uint8** peut poser problème. Il est donc préférable d'utiliser **float32** avant la normalisation afin de préserver la précision des calculs.

## Question 4 POR FAZER(ARRUMAR O CÓDIGO POIS N TEM DIFERENCA PRA DIFERENTES ÉCHELLES(S))

La valeur de θ nous indique si la direction du pixel est classifiée comme plat (flat), coin (corner) ou bord (edge).

- Si θ est fortement positif, le pixel est classé comme un coin (corner).
- Si θ est négatif, il est classé comme un bord (edge).
- Si θ est légèrement positif, il est classé comme une région plate (flat).

Cela représente uniquement l'analyse de θ. Sa valeur provient principalement du calcul du gradient avec le filtre de Sobel, qui permet d'identifier les variations selon les axes X et Y. Ensuite, un filtre gaussien est appliqué pour réduire le bruit (blur). Enfin, nous calculons les valeurs propes de la matrice M, qui nous aidera à déterminer si un point correspond à un coin (corner) ou non. 

https://medium.com/@itberrios6/harris-corner-and-edge-detector-4169312aa2f8

## Question 5  POR FAZER


+alpha, -points --> Theta va diminuer --> Moins de points que sont plus important que le treshold
+taille fenetre, - points --> Simpler images generated, less blur to exclude
+s, + points --> **Nao sei, n parece estar tendo mt efeito igual ao livro, precisa reve**

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

## Question 7 FEITO

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
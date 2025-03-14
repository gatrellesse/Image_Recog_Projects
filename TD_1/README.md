#TD1
### Question 1

#### NAO FIZ SOBRE AS FUNCOES DO MATPLOT

## Comparaison des performances
Nous observons une différence significative en termes de temps de calcul entre les deux méthodes. En utilisant le filtre 2D d'OpenCV, nous obtenons des résultats environ **100 fois plus rapides** que la méthode directe.

### Explication de l'optimisation
- **Petits noyaux** : OpenCV optimise la convolution en exploitant la décomposition d'un noyau $ k = r \times c $, réduisant ainsi la complexité de $ O(MNPQ) $ à $ O(MN(P+Q)) $.
- **Grands noyaux** : Il utilise le **théorème de convolution**, qui convertit la convolution dans le domaine spatial en une multiplication dans le domaine fréquentiel, avec une implémentation FFT efficace :
  
  $$ f * g = \mathcal{F}^{-1}(\mathcal{F}(f) \cdot \mathcal{F}(g)) $$
  
  Ce qui réduit la complexité de $ O(MNPQ) $ à $ O((M+K) \cdot (N+L) \cdot \log(M+K) \cdot \log(N+L)) $.

### Question 2

Le noyau effectue un **rehaussement de contraste**, mais en réalité, il s'agit simplement de la suppression du flou(blur) de l'image originale.  

Le kernel utilisé est obtenu en appliquant la formule suivante :  $\text{résultat} = \text{original} + (\text{original} - \text{blur}) \times \text{facteur}$ où le flou est généré à l'aide d'un filtre non linéaire, plus précisément le **filtre gaussien**, qui agit comme un **filtre passe-bas**.

### Question 3

#### O RESULTADO IX IY E NORM ta sendo plotado, colocar no rapport dps

Puisque la convolution approxime les composantes des gradients des dérivées partielles, nous allons utiliser les opérateurs de Sobel pour calculer $ I_x $, $ I_y $ et, par conséquent, la norme du gradient.  

Il faut faire attention à l'affichage, car ces trois résultats peuvent contenir des valeurs négatives ou des valeurs positives hors de l'intervalle utilisé par les fonctions de Matplotlib, qui attendent des valeurs entre **[0, 255]**. Par conséquent, nous allons normaliser ces valeurs dans cet intervalle.  

Une dernière remarque sur le type de données du gradient : comme il peut contenir des valeurs négatives, l'utilisation du type **uint8** peut poser problème. Il est donc préférable d'utiliser **float32** avant la normalisation afin de préserver la précision des calculs.

### Question 4
https://medium.com/@itberrios6/harris-corner-and-edge-detector-4169312aa2f8

### Question 5
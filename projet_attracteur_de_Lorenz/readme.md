# Objectif : Etude et affichage de l'attracteur de Lorenz.

## Etude
![alt text](https://github.com/pierreravenel/Mini-Projets-C/blob/master/projet_attracteur_de_Lorenz/out.PNG)

L’attracteur de Lorenz est une structure fractale correspondant au comportement à long terme de l'oscillateur de Lorenz. L'attracteur montre comment les différentes variables du système dynamique évoluent dans le temps en une trajectoire non périodique.

![alt text](https://wikimedia.org/api/rest_v1/media/math/render/svg/c32b412baae29eb2c914c397d425023188c3d433)

https://fr.wikipedia.org/wiki/Attracteur_de_Lorenz

Objectifs :

    1) Etude de l'attracteur de lorenz et resolution de ce système d'équations différentielles
       avec la méthode d'euler. 
       
    2) Représentation dans une fenetre graphique
    
    3) Ajout de perspective : VOIR PROJET CUBE ! (Equation de transfert)
    
    4) Ajout de mouvement (rotation autour d'un axe central) : (Nouveau)

## Résolution
méthode d'Euler :
Résolutuion avec la méthode d'euler d'un système d'équations différentielles

https://fr.wikipedia.org/wiki/M%C3%A9thode_d%27Euler
    
    Soit Xnew = Xlast + Xlast' * deltat
Tel que Xlast est la vitesse de X à cet instant. On l'applique sur un temps infinitésimal deltat

## Affichage
### Représentation dans une fenetre graphique
On affiche uniquement deux dimentinos de notre repere à trois dimentions.

### Ajout de perspective
On utilise la profondeur du point pour determiner sa position. Voir projet CUBE.

### Ajout de mouvement : rotation autour d'un axe central (Nouveauté)

Etudions comment effectuer la rotation d'un point autour d'un axe. C' est un peu comme dans le projet_CUBE, à l'inverse que l'on controle tous les point indépendament.

On se projette dans le repere plan orthogonal à l'axe de rotation, ici (0,x,y).

On détermine la distance de ce point par raport à l'origine. (Théorème de pytagore)
	r = sqrt(x²+y²)
On détermine "l'argument" de ce point (l'angle avec l'axe x sur [0:2pi])
	angle = arctan(y/x)
La position de ce point peux désormait être exprimer de la manière suivante :
	
	x(phi)=r*cos(angle + phi)
	y(phi)=r*sin(angle + phi)

Cette forme nous permet alors de faire tourner ce point autour de l'axe précedant en faisant varier phi.

On peux désormait appliquer les équation pour convertir ce point dans un repere 3D à un repère en deux dimentions. (Voir projet cube)
Notons une découverte : le "coefficient de déformation" est le même pour les deux equation. 


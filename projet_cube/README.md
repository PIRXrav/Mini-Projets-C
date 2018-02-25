# Objectif : dessiner un cube en 3D dans une fenêtre en deux dimentions avec la librairie SDL.

Soit un cube de coté c
Soit un de ses sommets : SM

Dans le repere (0,x,y,z) :

Appliquons à ce sommet l'equation parametrique suivante :

      x(phi)=cos(phi)*r

      y(phi)=sin(phi)*r

      z(phy)=CONST

Avec r le rayon du cercle passant par les 4 sommets haut du cube. Tel que : 

      r = sqrt(2*c²)/2;

Et z la hauteur des 4 sommets haut.

On crée un plan virtuelle R a une distance dist_plan_objet entre R et le centre du cube C'.
On crée un point virtuelle (la camera) C à une distance dist_camera_plan entre R et C

On observe alors la projection crée sur R du point suivant,
tel que : ce point est le croisement entre R et la droite CC'

dans le repere plan R(planx,plany):

      planx(phi)=-dist_camera_plan/(dist_camera_plan+dist_plan_objet+x(phi))*y(phi);

      plany(phi)=dist_camera_plan/(dist_camera_plan+dist_plan_objet+x(phi))*z(phi);
   
![alt text](https://github.com/pierreravenel/Mini-Projets-C/blob/master/projet_cube/equations.PNG)

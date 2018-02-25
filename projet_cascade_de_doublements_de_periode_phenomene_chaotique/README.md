# Objectif : Etude d'un phenomene chaotique & Génération de "monde" à partir d'une clée (xini) qui sont totalement différents en cas de variation de cette clée

https://fr.wikipedia.org/wiki/Th%C3%A9orie_du_chaos

=== Cascade de doublements de période ===

    Soit : x_{n+1}  = r * x_n * (1 - x_n)

où n = 0, 1, …'' dénote le temps discret, x l'unique variable dynamique, et 0 < r < 4 un paramètre.

Lorsque le paramètre r devient supérieur à quatre, l'application sort de l'intervalle [0, 1].
La dynamique de cette application présente un comportement très différent selon la valeur du paramètre r :
* Pour 0 < r < 3 , le système possède un point fixe attractif, qui devient instable lorsque r = 3
* Pour 3 < r < 3.57..., l'application possède un attracteur qui est une orbite périodique, de période 2^n où n est un entier qui tend vers l'infini lorsque r 3.57…
* Lorsque r = 3.57..., l'application possède un attracteur chaotique fractal
* Le cas r = 4 À noter qu'on peut dans ce cas précis établir l'expression exacte de la mesure invariante

Lorsque le paramètre r augmente, on obtient donc une succession de bifurcations entre les comportements périodiques et le chaos :
https://upload.wikimedia.org/wikipedia/commons/7/7d/LogisticMap_BifurcationDiagram.png

![alt text](https://github.com/pierreravenel/Mini-Projets-C/blob/master/projet_cascade_de_doublements_de_periode_phenomene_chaotique/exemple_style_lunary_compresse.PNG)

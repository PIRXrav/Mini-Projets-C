# Objectif : Etude d'un phenomene chaotique & Génération de "monde" à partir d'une clée (xini) qui sont totalement différents en cas de variation de cette clée

![alt text](https://github.com/pierreravenel/Mini-Projets-C/blob/master/projet_cascade_de_doublements_de_periode_phenomene_chaotique/exemple_style_lunary_compresse.PNG)

https://fr.wikipedia.org/wiki/Th%C3%A9orie_du_chaos

=== Cascade de doublements de période ===

    x_{n+1}  = r * x_n * (1 - x_n)



où n = 0, 1, …'' dénote le temps discret, x l'unique variable dynamique, et 0 < r < 4 un paramètre.

Lorsque le paramètre r devient supérieur à quatre, l'application sort de l'intervalle [0, 1].
La dynamique de cette application présente un comportement très différent selon la valeur du paramètre r :
* Pour 0 < r < 3 , le système possède un point fixe attractif, qui devient instable lorsque r = 3
* Pour 3 < r < 3.57..., l'application possède un attracteur qui est une orbite périodique, de période 2^n où n est un entier qui tend vers l'infini lorsque r 3.57…
* Lorsque r = 3.57..., l'application possède un attracteur chaotique fractal
* Le cas r = 4 À noter qu'on peut dans ce cas précis établir l'expression exacte de la mesure invariante

Lorsque le paramètre r augmente, on obtient donc une succession de bifurcations entre les comportements périodiques et le chaos :
https://upload.wikimedia.org/wikipedia/commons/7/7d/LogisticMap_BifurcationDiagram.png


# Using Inverse Probability Weighting with SVMs to Address Confounding
### by Kristin A. Linn
### June 25, 2015

Here we provide an example of how to implement inverse probability weighting with SVMs to address confounding.  The basic setup is that we have feature, class label pairs of the form ($x_i, y_i$) for each subject, i=1,...,n, where $y_i \in \{0, 1\}$ and $x_i \in R^p$ for all $i$. We wish to train a SVM to predict y given x. As an example, y might be an indicator of disease/control group and x might be a vectorized image containing voxel values or volumes of regions across the brain. However, the additional feature vector $a_i \in R^s$ observed for all subjects confounds the relationship between x and y. For example, a might contain covariates such as age and sex.  In the presence of confounding by a, inverse probability weighting is used to recover an estimate of the target classifier, which is the SVM classifier that would have been estimated had there been no confounding by a.


```{r}
set.seed (1)
```

We use the package 'rPython' to access libSVM (https://www.csie.ntu.edu.tw/~cjlin/libsvm/) through scikit learn (http://scikit-learn.org/stable/). The file fit_svm.py contains a python function that implements a linear kernel SVM with subject-level weights and a grid search to tune the cost parameter, C.


```{r}
print("I am R Code")
```

## Generate data for example

We generate data such that the confounders, a1 and a2, affect both the features, x1 and x2, as well as the class labels, d.


N.B. If some of the estimated weights are extremely large, one may consider truncating the predicted probabilities (e.g., at the 5th percentile) or using stabilized weights. Define $S_i = pr(d_i=1 | a1_i, a2_i)$ and $M_i = pr(d_i=1)$ as well as corresponding estimates \hat{S}_i = \hat{pr}(d_i=1 | a1_i, a2_i) and \hat{M}_i = \hat{pr}(d_i=1), Then, stabilized weights and their corresponding estimates are defined, respectively, as:
$$
W*_i = d_{i}*(M_i/S_i) + (1-d_{i})*(1-M_i)/(1-S_i) 
$$
and fun
$$
\hat{W*}_i = d_{i}*(\hat{M}_i/\hat{S}_i) + (1-d_{i})*(1-\hat{M}_i)/(1-\hat{S}_i) 
$$
## Train the inverse probability weighted SVM (IPW-SVM)


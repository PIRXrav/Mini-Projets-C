# Convertisseur base 64
En informatique, base64 est un codage de l'information utilisant 64 caractères. Ll est principalement utilisé pour la transmission de messages (courrier électronique et forums Usenet) sur Internet.
## Etude
Un alphabet de 65 caractères est utilisé pour permettre la représentation de 6 bits par un caractère. Le 65e caractère (signe «=») n'est utilisé qu'en complément final dans le processus de codage d'un message.

Ce processus de codage consiste à coder chaque groupe de 24 bits successifs de données par une chaîne de 4 caractères. On procède de gauche à droite, en concaténant 3 octets pour créer un seul groupement de 24 bits (8 bits par octet). Ils sont alors séparés en 4 nombres de seulement 6 bits (qui en binaire ne permettent que 64 combinaisons). Chacune des 4 valeurs est enfin représentée (codée) par un caractère de l'alphabet retenu. (Table ci-dessous.)

| Index | Char | Index | Char | Index | Char | Index | Char |
|------:|------|------:|------|------:|------|------:|------|
|     0 | A    |    16 | Q    |    32 | g    |    48 | w    |
|     1 | B    |    17 | R    |    33 | h    |    49 | x    |
|     2 | C    |    18 | S    |    34 | i    |    50 | y    |
|     3 | D    |    19 | T    |    35 | j    |    51 | z    |
|     4 | E    |    20 | U    |    36 | k    |    52 | 0    |
|     5 | F    |    21 | V    |    37 | l    |    53 | 1    |
|     6 | G    |    22 | W    |    38 | m    |    54 | 2    |
|     7 | H    |    23 | X    |    39 | n    |    55 | 3    |
|     8 | I    |    24 | Y    |    40 | o    |    56 | 4    |
|     9 | J    |    25 | Z    |    41 | p    |    57 | 5    |
|    10 | K    |    26 | a    |    42 | q    |    58 | 6    |
|    11 | L    |    27 | b    |    43 | r    |    59 | 7    |
|    12 | M    |    28 | c    |    44 | s    |    60 | 8    |
|    13 | N    |    29 | d    |    45 | t    |    61 | 9    |
|    14 | O    |    30 | e    |    46 | u    |    62 | +    |
|    15 | P    |    31 | f    |    47 | v    |    63 | /    |

https://fr.wikipedia.org/wiki/Base64

## Usage
   Protocole SMTP (à venir)
## Exemple
    encode("Hello world") -> SGVsbG8gd29ybGQ=
    decode("SGVsbG8gd29ybGQ=") -> Hello world

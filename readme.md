## François Gibier - Microprocesseur

## Compilation

Pour compiler le projet, il faut éxécuter la commande:
```bash
make
```

## Execution

Pour lancer le projet, il faut éxécuter la commande:
```bash
./main nom_prog flags
```
Les programmes sont dans le dossier progs, il y en a 4, deux qui chargent des données dans la mémoire pour après être utilisées dans les programmes d'éxécution. Il faut donc chaîner les programmes si on veut charger les opérandes pour le programme de multiplication par exemple, ou pour mettre des données dans la mémoire pour le memcopy.

### Exemples

#### Memcopy
```bash
./main progs/opmemcopy.ms progs/memcopy.ms
```

#### Mult
```bash
./main progs/opmult.ms progs/mult.ms
```

## Parser en Assembleur

Pour parser le programme en assembleur, il faut ajouter le flag **-s** à la ligne de commande.
Le fichier parser s'appelle **program.ms**

## Debugger

Pour éxécuter le programme avec le debugger, il faut ajouter le flag **-d** à la ligne de commande.

On a ensuite différentes commandes disponibles (un peu comme gdb):
- break XXXX -> avec X un caractère hexadécimal, break à une adresse mémoire, il est possible de faire plusieurs breaks comme dans gdb
- run -> éxécute le programme jusqu'au prochain breakpoint, si il y en a un, sinon jusqu'à la fin du programme
- info-registers -> affiche les informations sur les registres
- info-ram -> affiche toute la ram
- end -> termine le programme

## Cycles

Je compte le nombre de cycles dont le microprocesseur à besoin pour éxécuter le programme, en supposant que read et write prennent 3 cycles et que toutes les autres micro-instructions prennent un cycle.

## Tests

Il y a quelques tests dans le dossier ***tests***, pour le JMP, JZ, JC, LD, ST.

On peut éxécuter les tests après avoir compilé le projet avec la commande:
```bash
./test
```